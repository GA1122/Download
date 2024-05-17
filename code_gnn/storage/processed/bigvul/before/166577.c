void BrowserCommandController::UpdateShowSyncState(bool show_main_ui) {
  if (is_locked_fullscreen_)
    return;

  command_updater_.UpdateCommandEnabled(
      IDC_SHOW_SIGNIN, show_main_ui && pref_signin_allowed_.GetValue());
}
