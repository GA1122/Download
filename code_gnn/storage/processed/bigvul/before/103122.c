void Browser::Home(WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(UserMetricsAction("Home"), profile_);
  OpenURL(GetHomePage(), GURL(), disposition, PageTransition::AUTO_BOOKMARK);
}
