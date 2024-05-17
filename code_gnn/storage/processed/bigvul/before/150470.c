void NavigationControllerImpl::LoadEntry(
    std::unique_ptr<NavigationEntryImpl> entry) {
  last_pending_entry_ = pending_entry_;
  last_pending_entry_index_ = pending_entry_index_;
  last_transient_entry_index_ = transient_entry_index_;
  pending_entry_ = nullptr;
  SetPendingEntry(std::move(entry));
  NavigateToPendingEntry(ReloadType::NONE);
}
