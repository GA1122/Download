NavigationEntryImpl* NavigationControllerImpl::GetVisibleEntry() const {
  if (transient_entry_index_ != -1)
    return entries_[transient_entry_index_].get();
  bool safe_to_show_pending =
      pending_entry_ &&
      pending_entry_index_ == -1 &&
      (!pending_entry_->is_renderer_initiated() || IsUnmodifiedBlankTab());

  if (!safe_to_show_pending &&
      pending_entry_ &&
      pending_entry_index_ != -1 &&
      IsInitialNavigation() &&
      !pending_entry_->is_renderer_initiated())
    safe_to_show_pending = true;

  if (safe_to_show_pending)
    return pending_entry_;
  return GetLastCommittedEntry();
}
