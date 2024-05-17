void Browser::ShowHelpTab() {
  UserMetrics::RecordAction(UserMetricsAction("ShowHelpTab"));
  GURL help_url(kHelpContentUrl);
  GURL localized_help_url = google_util::AppendGoogleLocaleParam(help_url);
  ShowSingletonTab(localized_help_url);
}
