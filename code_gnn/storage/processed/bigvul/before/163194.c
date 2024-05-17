int NavigationControllerImpl::GetLastCommittedEntryIndex() const {
  DCHECK_LT(last_committed_entry_index_, GetEntryCount());
  DCHECK(GetEntryCount() || last_committed_entry_index_ == -1);
  return last_committed_entry_index_;
}
