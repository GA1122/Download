bool Browser::PreHandleKeyboardEvent(content::WebContents* source,
                                     const NativeWebKeyboardEvent& event,
                                     bool* is_keyboard_shortcut) {
  if (event.windowsKeyCode == 27 &&
      fullscreen_controller_->HandleUserPressedEscape()) {
    return true;
  }
  return window()->PreHandleKeyboardEvent(event, is_keyboard_shortcut);
}
