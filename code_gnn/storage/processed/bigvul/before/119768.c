int NavigationControllerImpl::GetEntryCount() const {
  DCHECK(entries_.size() <= max_entry_count());
  return static_cast<int>(entries_.size());
}
