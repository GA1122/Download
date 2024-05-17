NavigationEntryImpl* NavigationControllerImpl::GetPendingEntry() const {
  DCHECK(pending_entry_ || pending_entry_index_ == -1);

  DCHECK(pending_entry_index_ == -1 ||
         pending_entry_ == GetEntryAtIndex(pending_entry_index_));

  return pending_entry_;
}
