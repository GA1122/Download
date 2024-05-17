std::string GetKeyboardLayout() {
  return GetAccessibilityKeyboardEnabled() ? "system-qwerty" : "qwerty";
}
