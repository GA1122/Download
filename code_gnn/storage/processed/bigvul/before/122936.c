void RenderWidgetHostImpl::AddKeyboardListener(KeyboardListener* listener) {
  keyboard_listeners_.push_back(listener);
}
