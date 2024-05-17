void RenderWidgetHostImpl::RemoveKeyPressEventCallback(
    const KeyPressEventCallback& callback) {
  for (size_t i = 0; i < key_press_event_callbacks_.size(); ++i) {
    if (key_press_event_callbacks_[i].Equals(callback)) {
      key_press_event_callbacks_.erase(
          key_press_event_callbacks_.begin() + i);
      return;
    }
  }
}
