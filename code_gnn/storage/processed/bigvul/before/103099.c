void Browser::EmailPageLocation() {
  UserMetrics::RecordAction(UserMetricsAction("EmailPageLocation"), profile_);
  GetSelectedTabContents()->EmailPageLocation();
}
