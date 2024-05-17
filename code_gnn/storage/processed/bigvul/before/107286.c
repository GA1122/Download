void Browser::CloseTab() {
  UserMetrics::RecordAction(UserMetricsAction("CloseTab_Accelerator"),
                            profile_);
  if (CanCloseTab()) {
    tab_handler_->GetTabStripModel()->CloseTabContentsAt(
        tab_handler_->GetTabStripModel()->selected_index(),
        TabStripModel::CLOSE_USER_GESTURE |
        TabStripModel::CLOSE_CREATE_HISTORICAL_TAB);
  }
}
