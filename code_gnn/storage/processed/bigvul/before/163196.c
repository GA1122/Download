int NavigationControllerImpl::GetPendingEntryIndex() const {
  DCHECK_LT(pending_entry_index_, GetEntryCount());
  DCHECK(GetEntryCount() != 0 || pending_entry_index_ == -1);
  return pending_entry_index_;
}
