void WebContentsImpl::RemoveAccessibilityMode(AccessibilityMode mode) {
  SetAccessibilityMode(RemoveAccessibilityModeFrom(accessibility_mode_, mode));
}
