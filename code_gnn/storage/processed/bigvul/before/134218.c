KeyboardCode KeyboardCodeFromNative(const base::NativeEvent& native_event) {
  return KeyboardCodeForWindowsKeyCode(native_event.wParam);
}
