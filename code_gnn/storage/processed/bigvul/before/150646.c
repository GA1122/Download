InputHandlerProxy::EventDisposition InputHandlerProxy::HandleGestureScrollBegin(
    const WebGestureEvent& gesture_event) {
  TRACE_EVENT0("input", "InputHandlerProxy::HandleGestureScrollBegin");

  if (scroll_predictor_)
    scroll_predictor_->ResetOnGestureScrollBegin(gesture_event);

#if DCHECK_IS_ON()
  expect_scroll_update_end_ = true;
#endif
  cc::ScrollState scroll_state = CreateScrollStateForGesture(gesture_event);
  cc::InputHandler::ScrollStatus scroll_status;
  if (gesture_event.data.scroll_begin.delta_hint_units ==
      ui::input_types::ScrollGranularity::kScrollByPage) {
    scroll_status.thread = cc::InputHandler::SCROLL_ON_MAIN_THREAD;
    scroll_status.main_thread_scrolling_reasons =
        cc::MainThreadScrollingReason::kContinuingMainThreadScroll;
  } else if (gesture_event.data.scroll_begin.target_viewport) {
    scroll_status = input_handler_->RootScrollBegin(
        &scroll_state, GestureScrollInputType(gesture_event.SourceDevice()));
  } else if (ShouldAnimate(
                 gesture_event.data.scroll_begin.delta_hint_units !=
                 ui::input_types::ScrollGranularity::kScrollByPixel)) {
    DCHECK(!scroll_state.is_in_inertial_phase());
    scroll_status = input_handler_->ScrollAnimatedBegin(&scroll_state);
  } else {
    scroll_status = input_handler_->ScrollBegin(
        &scroll_state, GestureScrollInputType(gesture_event.SourceDevice()));
  }
  RecordMainThreadScrollingReasons(gesture_event.SourceDevice(),
                                   scroll_status.main_thread_scrolling_reasons);

  InputHandlerProxy::EventDisposition result = DID_NOT_HANDLE;
  scroll_sequence_ignored_ = false;
  in_inertial_scrolling_ = false;
  switch (scroll_status.thread) {
    case cc::InputHandler::SCROLL_ON_IMPL_THREAD:
      TRACE_EVENT_INSTANT0("input", "Handle On Impl", TRACE_EVENT_SCOPE_THREAD);
      gesture_scroll_on_impl_thread_ = true;
      if (input_handler_->IsCurrentlyScrollingViewport())
        client_->DidStartScrollingViewport();

      if (scroll_status.bubble)
        result = DID_HANDLE_SHOULD_BUBBLE;
      else
        result = DID_HANDLE;
      break;
    case cc::InputHandler::SCROLL_UNKNOWN:
    case cc::InputHandler::SCROLL_ON_MAIN_THREAD:
      TRACE_EVENT_INSTANT0("input", "Handle On Main", TRACE_EVENT_SCOPE_THREAD);
      result = DID_NOT_HANDLE;
      break;
    case cc::InputHandler::SCROLL_IGNORED:
      TRACE_EVENT_INSTANT0("input", "Ignore Scroll", TRACE_EVENT_SCOPE_THREAD);
      scroll_sequence_ignored_ = true;
      result = DROP_EVENT;
      break;
  }
  if (scroll_elasticity_controller_ && result != DID_NOT_HANDLE)
    HandleScrollElasticityOverscroll(gesture_event,
                                     cc::InputHandlerScrollResult());

  return result;
}
