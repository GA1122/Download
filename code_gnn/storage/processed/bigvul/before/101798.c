void Browser::SelectNextTab() {
  UserMetrics::RecordAction(UserMetricsAction("SelectNextTab"));
  tab_handler_->GetTabStripModel()->SelectNextTab();
}
