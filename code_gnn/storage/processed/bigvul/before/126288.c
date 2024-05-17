void BrowserCommandController::TabRestoreServiceChanged(
    TabRestoreService* service) {
  command_updater_.UpdateCommandEnabled(IDC_RESTORE_TAB,
                                        CanRestoreTab(browser_));
}
