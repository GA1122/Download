void Browser::OpenImportSettingsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("Import_ShowDlg"));
  ShowOptionsTab(chrome::kImportDataSubPage);
}
