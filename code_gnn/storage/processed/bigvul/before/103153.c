void Browser::OpenImportSettingsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("Import_ShowDlg"), profile_);
  ShowOptionsTab(chrome::kImportDataSubPage);
}
