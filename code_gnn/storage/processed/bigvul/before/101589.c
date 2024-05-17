bool Browser::CanRestoreTab() {
  return command_updater_.IsCommandEnabled(IDC_RESTORE_TAB);
}
