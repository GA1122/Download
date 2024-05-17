void Browser::UpdatePrintingState(int content_restrictions) {
  bool enabled = true;
  bool selected_tab_is_preview_tab = false;
  if (content_restrictions & CONTENT_RESTRICTION_PRINT) {
    enabled = false;
    selected_tab_is_preview_tab =
        printing::PrintPreviewTabController::IsPrintPreviewTab(
            GetSelectedTabContentsWrapper());
  } else if (g_browser_process->local_state()) {
    enabled = g_browser_process->local_state()->
        GetBoolean(prefs::kPrintingEnabled);
  }
  command_updater_.UpdateCommandEnabled(IDC_PRINT, enabled);
  command_updater_.UpdateCommandEnabled(IDC_ADVANCED_PRINT,
                                        selected_tab_is_preview_tab ? true :
                                                                      enabled);
}
