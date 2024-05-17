void BrowserCommandController::UpdateReloadStopState(bool is_loading,
                                                     bool force) {
  window()->UpdateReloadStopState(is_loading, force);
  command_updater_.UpdateCommandEnabled(IDC_STOP, is_loading);
}
