void LockContentsView::OnFocusLeavingSystemTray(bool reverse) {
  FindFirstOrLastFocusableChild(this, reverse)->RequestFocus();

  if (lock_screen_apps_active_) {
    Shell::Get()->login_screen_controller()->FocusLockScreenApps(reverse);
    return;
  }
}
