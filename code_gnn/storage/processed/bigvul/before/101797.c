void Browser::SelectLastTab() {
  UserMetrics::RecordAction(UserMetricsAction("SelectLastTab"));
  tab_handler_->GetTabStripModel()->SelectLastTab();
}
