void Browser::SelectTabContentsAt(int index, bool user_gesture) {
  tab_handler_->GetTabStripModel()->SelectTabContentsAt(index, user_gesture);
}
