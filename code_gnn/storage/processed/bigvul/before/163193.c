int NavigationControllerImpl::GetEntryCount() const {
  DCHECK_LE(entries_.size(), max_entry_count());
  return static_cast<int>(entries_.size());
}
