void Browser::CloseTab() {
  UserMetrics::RecordAction(UserMetricsAction("CloseTab_Accelerator"));
  if (CanCloseTab())
    tab_handler_->GetTabStripModel()->CloseSelectedTabs();
}
