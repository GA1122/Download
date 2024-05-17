void BrowserCommandController::UpdateOpenFileState() {
  bool enabled = true;
  PrefService* local_state = g_browser_process->local_state();
  if (local_state)
    enabled = local_state->GetBoolean(prefs::kAllowFileSelectionDialogs);

  command_updater_.UpdateCommandEnabled(IDC_OPEN_FILE, enabled);
}
