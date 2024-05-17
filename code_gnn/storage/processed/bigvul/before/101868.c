void Browser::UpdateSaveAsState(int content_restrictions) {
  bool enabled = !(content_restrictions & CONTENT_RESTRICTION_SAVE);
  PrefService* state = g_browser_process->local_state();
  if (state)
    enabled = enabled && state->GetBoolean(prefs::kAllowFileSelectionDialogs);

  command_updater_.UpdateCommandEnabled(IDC_SAVE_PAGE, enabled);
}
