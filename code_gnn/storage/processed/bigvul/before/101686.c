void Browser::Home(WindowOpenDisposition disposition) {
  UserMetrics::RecordAction(UserMetricsAction("Home"));
  OpenURL(GetHomePage(), GURL(), disposition,
          PageTransition::AUTO_BOOKMARK | PageTransition::HOME_PAGE);
}
