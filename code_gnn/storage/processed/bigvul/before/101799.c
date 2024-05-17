void Browser::SelectNumberedTab(int index) {
  if (index < tab_count()) {
    UserMetrics::RecordAction(UserMetricsAction("SelectNumberedTab"));
    tab_handler_->GetTabStripModel()->ActivateTabAt(index, true);
  }
}
