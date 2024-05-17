NavigationEntry* NavigationController::GetVisibleEntry() const {
  if (transient_entry_index_ != -1)
    return entries_[transient_entry_index_].get();
  if (pending_entry_ &&
      pending_entry_->page_id() == -1 &&
      !pending_entry_->is_renderer_initiated())
    return pending_entry_;
  return GetLastCommittedEntry();
}
