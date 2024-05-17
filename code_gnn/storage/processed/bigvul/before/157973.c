bool RenderViewImpl::RenderWidgetWillHandleMouseEventForWidget(
    const blink::WebMouseEvent& event) {
  return GetWidget()->mouse_lock_dispatcher()->WillHandleMouseEvent(event);
}
