void InfoBar::InfoBarAdded() {
  DCHECK(container_);
#if defined(OS_WIN)
  views::Widget* widget = GetWidget();
  if (widget) {
    focus_tracker_.reset(new views::ExternalFocusTracker(this,
                                                         GetFocusManager()));
  }
#endif

  if (GetFocusManager())
    GetFocusManager()->AddFocusChangeListener(this);

  NotifyAccessibilityEvent(AccessibilityTypes::EVENT_ALERT);
}
