bool RenderViewImpl::RenderWidgetWillHandleMouseEvent(
    const blink::WebMouseEvent& event) {
  return mouse_lock_dispatcher_->WillHandleMouseEvent(event);
}
