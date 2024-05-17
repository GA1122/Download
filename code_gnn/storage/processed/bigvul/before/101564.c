void Browser::ActivateTabAt(int index, bool user_gesture) {
  tab_handler_->GetTabStripModel()->ActivateTabAt(index, user_gesture);
}
