void Browser::Print() {
  UserMetrics::RecordAction(UserMetricsAction("PrintPreview"), profile_);
  GetSelectedTabContents()->PrintPreview();
}
