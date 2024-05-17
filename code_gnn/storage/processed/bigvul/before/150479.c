bool NavigationControllerImpl::PendingEntryMatchesHandle(
    NavigationHandleImpl* handle) const {
  return pending_entry_ &&
         pending_entry_->GetUniqueID() == handle->pending_nav_entry_id();
}
