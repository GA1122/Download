void WebContentsImpl::AddAccessibilityMode(AccessibilityMode mode) {
  AccessibilityMode new_mode(accessibility_mode_);
  new_mode |= mode;
  SetAccessibilityMode(new_mode);
}
