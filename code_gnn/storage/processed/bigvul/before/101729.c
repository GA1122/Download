void Browser::OpenBugReportDialog() {
  UserMetrics::RecordAction(UserMetricsAction("ReportBug"));
  browser::ShowHtmlBugReportView(this, std::string(), 0);
}
