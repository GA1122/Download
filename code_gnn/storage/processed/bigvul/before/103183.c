void Browser::SelectLastTab() {
  UserMetrics::RecordAction(UserMetricsAction("SelectLastTab"), profile_);
  tab_handler_->GetTabStripModel()->SelectLastTab();
}
