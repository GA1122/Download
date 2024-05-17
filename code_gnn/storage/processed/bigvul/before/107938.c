void InfoBar::FocusWillChange(View* focused_before, View* focused_now) {
  if (focused_before && focused_now &&
      !this->IsParentOf(focused_before) && this->IsParentOf(focused_now)) {
    NotifyAccessibilityEvent(AccessibilityTypes::EVENT_ALERT);
  }
}
