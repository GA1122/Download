void Browser::FocusSearch() {
  UserMetrics::RecordAction(UserMetricsAction("FocusSearch"));
  window_->GetLocationBar()->FocusSearch();
}
