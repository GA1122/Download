void RenderWidgetHostImpl::RemoveKeyboardListener(
    KeyboardListener* listener) {
  keyboard_listeners_.remove(listener);
}
