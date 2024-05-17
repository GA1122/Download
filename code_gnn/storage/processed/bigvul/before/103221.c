void Browser::UpdatePrintingState(int content_restrictions) {
  bool enabled = true;
  if (content_restrictions & CONTENT_RESTRICTION_PRINT) {
    enabled = false;
  } else if (g_browser_process->local_state()) {
    enabled = printing_enabled_.GetValue();
  }
  command_updater_.UpdateCommandEnabled(IDC_PRINT, enabled);
}
