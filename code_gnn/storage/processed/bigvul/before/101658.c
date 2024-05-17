int Browser::GetDragActions() const {
  return TabStripModelDelegate::TAB_TEAROFF_ACTION | (tab_count() > 1 ?
      TabStripModelDelegate::TAB_MOVE_ACTION : 0);
}
