void NavigationControllerImpl::SetPendingEntry(NavigationEntryImpl* entry) {
  DiscardNonCommittedEntriesInternal();
  pending_entry_ = entry;
  NotificationService::current()->Notify(
      NOTIFICATION_NAV_ENTRY_PENDING,
      Source<NavigationController>(this),
      Details<NavigationEntry>(entry));
}
