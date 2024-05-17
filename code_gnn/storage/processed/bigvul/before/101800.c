void Browser::SelectPreviousTab() {
  UserMetrics::RecordAction(UserMetricsAction("SelectPrevTab"));
  tab_handler_->GetTabStripModel()->SelectPreviousTab();
}
