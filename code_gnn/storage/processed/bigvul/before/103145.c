void Browser::OpenBugReportDialog() {
  UserMetrics::RecordAction(UserMetricsAction("ReportBug"), profile_);
  browser::ShowHtmlBugReportView(this);
}
