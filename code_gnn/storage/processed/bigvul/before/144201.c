void LockContentsView::OnFocusLeavingLockScreenApps(bool reverse) {
  if (!reverse || lock_screen_apps_active_)
    FocusNextWidget(reverse);
  else
    FindFirstOrLastFocusableChild(this, reverse)->RequestFocus();
}
