void Browser::SelectNumberedTab(int index) {
  if (index < tab_count()) {
    UserMetrics::RecordAction(UserMetricsAction("SelectNumberedTab"),
                              profile_);
    tab_handler_->GetTabStripModel()->ActivateTabAt(index, true);
  }
}
