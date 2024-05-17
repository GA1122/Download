void Browser::OpenClearBrowsingDataDialog() {
  UserMetrics::RecordAction(UserMetricsAction("ClearBrowsingData_ShowDlg"));
  ShowOptionsTab(chrome::kClearBrowserDataSubPage);
}
