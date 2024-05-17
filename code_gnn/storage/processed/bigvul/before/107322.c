void Browser::OpenImportSettingsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("Import_ShowDlg"), profile_);
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableTabbedOptions)) {
    ShowOptionsTab(chrome::kImportDataSubPage);
  } else {
    window_->ShowImportDialog();
  }
}
