void NavigationController::LoadEntry(NavigationEntry* entry) {
  if (HandleNonNavigationAboutURL(entry->url()))
    return;

  DiscardNonCommittedEntriesInternal();
  pending_entry_ = entry;
  NotificationService::current()->Notify(
      NotificationType::NAV_ENTRY_PENDING,
      Source<NavigationController>(this),
      NotificationService::NoDetails());
  NavigateToPendingEntry(false);
}
