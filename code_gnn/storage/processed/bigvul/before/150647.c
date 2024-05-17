InputHandlerProxy::EventDisposition InputHandlerProxy::HandleGestureScrollEnd(
  const WebGestureEvent& gesture_event) {
  TRACE_EVENT0("input", "InputHandlerProxy::HandleGestureScrollEnd");
#if DCHECK_IS_ON()
  DCHECK(expect_scroll_update_end_);
  expect_scroll_update_end_ = false;
#endif

  cc::ScrollState scroll_state = CreateScrollStateForGesture(gesture_event);
  input_handler_->ScrollEnd(&scroll_state, true);

  if (scroll_sequence_ignored_)
    return DROP_EVENT;

  if (!gesture_scroll_on_impl_thread_)
    return DID_NOT_HANDLE;

  if (scroll_elasticity_controller_)
    HandleScrollElasticityOverscroll(gesture_event,
                                     cc::InputHandlerScrollResult());

  gesture_scroll_on_impl_thread_ = false;
  return DID_HANDLE;
}
