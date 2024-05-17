InputHandlerProxy::HandleGestureScrollUpdate(
    const WebGestureEvent& gesture_event) {
#if DCHECK_IS_ON()
  DCHECK(expect_scroll_update_end_);
#endif

  gfx::Vector2dF scroll_delta(-gesture_event.data.scroll_update.delta_x,
                              -gesture_event.data.scroll_update.delta_y);
  TRACE_EVENT2("input", "InputHandlerProxy::HandleGestureScrollUpdate", "dx",
               scroll_delta.x(), "dy", scroll_delta.y());

  if (scroll_sequence_ignored_) {
    TRACE_EVENT_INSTANT0("input", "Scroll Sequence Ignored",
                         TRACE_EVENT_SCOPE_THREAD);
    return DROP_EVENT;
  }

  if (!gesture_scroll_on_impl_thread_ && !gesture_pinch_in_progress_)
    return DID_NOT_HANDLE;

  cc::ScrollState scroll_state = CreateScrollStateForGesture(gesture_event);
  in_inertial_scrolling_ = scroll_state.is_in_inertial_phase();
  gfx::PointF scroll_point(gesture_event.PositionInWidget());

  if (ShouldAnimate(gesture_event.data.scroll_update.delta_units !=
                    ui::input_types::ScrollGranularity::kScrollByPixel)) {
    DCHECK(!scroll_state.is_in_inertial_phase());
    base::TimeTicks event_time = gesture_event.TimeStamp();
    base::TimeDelta delay = base::TimeTicks::Now() - event_time;
    switch (input_handler_
                ->ScrollAnimated(gfx::ToFlooredPoint(scroll_point),
                                 scroll_delta, delay)
                .thread) {
      case cc::InputHandler::SCROLL_ON_IMPL_THREAD:
        return DID_HANDLE;
      case cc::InputHandler::SCROLL_IGNORED:
        TRACE_EVENT_INSTANT0("input", "Scroll Ignored",
                             TRACE_EVENT_SCOPE_THREAD);
        return DROP_EVENT;
      case cc::InputHandler::SCROLL_ON_MAIN_THREAD:
      case cc::InputHandler::SCROLL_UNKNOWN:
        if (input_handler_->ScrollingShouldSwitchtoMainThread()) {
          TRACE_EVENT_INSTANT0("input", "Move Scroll To Main Thread",
                               TRACE_EVENT_SCOPE_THREAD);
          gesture_scroll_on_impl_thread_ = false;
          client_->GenerateScrollBeginAndSendToMainThread(gesture_event);
        }
        return DID_NOT_HANDLE;
    }
  }

  if (snap_fling_controller_->HandleGestureScrollUpdate(
          GetGestureScrollUpdateInfo(gesture_event))) {
#if DCHECK_IS_ON()
    expect_scroll_update_end_ = false;
#endif
    gesture_scroll_on_impl_thread_ = false;
    return DROP_EVENT;
  }

  cc::InputHandlerScrollResult scroll_result =
      input_handler_->ScrollBy(&scroll_state);

  if (!scroll_result.did_scroll &&
      input_handler_->ScrollingShouldSwitchtoMainThread()) {
    gesture_scroll_on_impl_thread_ = false;
    client_->GenerateScrollBeginAndSendToMainThread(gesture_event);

    if (!gesture_pinch_in_progress_)
      return DID_NOT_HANDLE;
  }

  HandleOverscroll(scroll_point, scroll_result);

  if (scroll_elasticity_controller_)
    HandleScrollElasticityOverscroll(gesture_event, scroll_result);

  return scroll_result.did_scroll ? DID_HANDLE : DROP_EVENT;
}
