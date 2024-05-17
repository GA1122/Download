bool RenderViewImpl::WillHandleMouseEvent(const WebKit::WebMouseEvent& event) {
  pepper_delegate_.WillHandleMouseEvent();

  return mouse_lock_dispatcher_->WillHandleMouseEvent(event);
}
