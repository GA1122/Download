void RenderWidgetHostImpl::RemoveMouseEventCallback(
    const MouseEventCallback& callback) {
  for (size_t i = 0; i < mouse_event_callbacks_.size(); ++i) {
    if (mouse_event_callbacks_[i].Equals(callback)) {
      mouse_event_callbacks_.erase(mouse_event_callbacks_.begin() + i);
      return;
    }
  }
}
