void Browser::FocusSearch() {
  UserMetrics::RecordAction(UserMetricsAction("FocusSearch"), profile_);
  window_->GetLocationBar()->FocusSearch();
}
