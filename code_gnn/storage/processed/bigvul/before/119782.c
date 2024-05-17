NavigationEntry* NavigationControllerImpl::GetVisibleEntry() const {
  if (transient_entry_index_ != -1)
    return entries_[transient_entry_index_].get();
  RenderViewHostImpl* rvh = static_cast<RenderViewHostImpl*>(
      web_contents_->GetRenderViewHost());
  bool safe_to_show_pending =
      pending_entry_ &&
      pending_entry_->GetPageID() == -1 &&
      (!pending_entry_->is_renderer_initiated() ||
       (IsInitialNavigation() &&
        !GetLastCommittedEntry() &&
        !rvh->has_accessed_initial_document()));

  if (!safe_to_show_pending &&
      pending_entry_ &&
      pending_entry_->GetPageID() != -1 &&
      IsInitialNavigation() &&
      !pending_entry_->is_renderer_initiated())
    safe_to_show_pending = true;

  if (safe_to_show_pending)
    return pending_entry_;
  return GetLastCommittedEntry();
}
