void Browser::CloseTab() {
  UserMetrics::RecordAction(UserMetricsAction("CloseTab_Accelerator"),
                            profile_);
  if (CanCloseTab())
    tab_handler_->GetTabStripModel()->CloseSelectedTabs();
}
