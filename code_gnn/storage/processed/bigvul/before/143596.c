void RenderWidgetHostImpl::AddKeyPressEventCallback(
    const KeyPressEventCallback& callback) {
  key_press_event_callbacks_.push_back(callback);
}
