void RenderWidgetHostImpl::AddMouseEventCallback(
    const MouseEventCallback& callback) {
  mouse_event_callbacks_.push_back(callback);
}
