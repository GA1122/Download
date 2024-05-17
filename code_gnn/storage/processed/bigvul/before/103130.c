void Browser::MoveTabPrevious() {
  UserMetrics::RecordAction(UserMetricsAction("MoveTabPrevious"), profile_);
  tab_handler_->GetTabStripModel()->MoveTabPrevious();
}
