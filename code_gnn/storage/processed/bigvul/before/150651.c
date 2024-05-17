void InputHandlerProxy::HandleOverscroll(
    const gfx::PointF& causal_event_viewport_point,
    const cc::InputHandlerScrollResult& scroll_result) {
  DCHECK(client_);
  if (!scroll_result.did_overscroll_root)
    return;

  TRACE_EVENT2("input",
               "InputHandlerProxy::DidOverscroll",
               "dx",
               scroll_result.unused_scroll_delta.x(),
               "dy",
               scroll_result.unused_scroll_delta.y());

  current_overscroll_params_.reset(new DidOverscrollParams());
  current_overscroll_params_->accumulated_overscroll =
      scroll_result.accumulated_root_overscroll;
  current_overscroll_params_->latest_overscroll_delta =
      scroll_result.unused_scroll_delta;
  current_overscroll_params_->causal_event_viewport_point =
      causal_event_viewport_point;
  current_overscroll_params_->overscroll_behavior =
      scroll_result.overscroll_behavior;
  return;
}
