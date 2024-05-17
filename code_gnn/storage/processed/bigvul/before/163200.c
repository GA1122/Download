void NavigationControllerImpl::LoadEntry(
    std::unique_ptr<NavigationEntryImpl> entry) {
  DCHECK_EQ(nullptr, last_pending_entry_);
  DCHECK_EQ(-1, last_pending_entry_index_);
  last_pending_entry_ = pending_entry_;
  last_pending_entry_index_ = pending_entry_index_;
  last_transient_entry_index_ = transient_entry_index_;

  pending_entry_ = nullptr;
  pending_entry_index_ = -1;
  SetPendingEntry(std::move(entry));
  NavigateToPendingEntry(ReloadType::NONE);
}
