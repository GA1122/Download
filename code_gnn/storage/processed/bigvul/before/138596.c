void NavigatorImpl::DiscardPendingEntryIfNeeded(NavigationHandleImpl* handle) {
  NavigationEntry* pending_entry = controller_->GetPendingEntry();
  bool pending_matches_fail_msg =
      handle && pending_entry &&
      handle->pending_nav_entry_id() == pending_entry->GetUniqueID();
  if (!pending_matches_fail_msg)
    return;

  bool should_preserve_entry = controller_->IsUnmodifiedBlankTab() ||
                               delegate_->ShouldPreserveAbortedURLs();
  if (pending_entry != controller_->GetVisibleEntry() ||
      !should_preserve_entry) {
    controller_->DiscardPendingEntry(true);

    controller_->delegate()->NotifyNavigationStateChanged(INVALIDATE_TYPE_URL);
  }
}
