int Browser::GetIndexForInsertionDuringRestore(int relative_index) {
  return (tab_handler_->GetTabStripModel()->insertion_policy() ==
      TabStripModel::INSERT_AFTER) ? tab_count() : relative_index;
}
