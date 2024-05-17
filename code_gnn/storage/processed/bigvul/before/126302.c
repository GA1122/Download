void BrowserCommandController::UpdateSaveAsState() {
  command_updater_.UpdateCommandEnabled(IDC_SAVE_PAGE, CanSavePage(browser_));
}
