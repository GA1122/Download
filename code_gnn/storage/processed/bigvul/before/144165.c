void LockContentsView::AboutToRequestFocusFromTabTraversal(bool reverse) {
  if (reverse && lock_screen_apps_active_) {
    Shell::Get()->login_screen_controller()->FocusLockScreenApps(reverse);
    return;
  }

  FocusNextWidget(reverse);
}
