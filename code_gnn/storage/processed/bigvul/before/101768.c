bool Browser::PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                     bool* is_keyboard_shortcut) {
  return window()->PreHandleKeyboardEvent(event, is_keyboard_shortcut);
}
