void Browser::MoveTabNext() {
  UserMetrics::RecordAction(UserMetricsAction("MoveTabNext"), profile_);
  tab_handler_->GetTabStripModel()->MoveTabNext();
}
