void Browser::SelectPreviousTab() {
  UserMetrics::RecordAction(UserMetricsAction("SelectPrevTab"), profile_);
  tab_handler_->GetTabStripModel()->SelectPreviousTab();
}
