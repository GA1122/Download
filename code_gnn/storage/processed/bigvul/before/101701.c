void Browser::MoveTabNext() {
  UserMetrics::RecordAction(UserMetricsAction("MoveTabNext"));
  tab_handler_->GetTabStripModel()->MoveTabNext();
}
