void NavigationController::NotifyNavigationEntryCommitted(
    LoadCommittedDetails* details,
    int extra_invalidate_flags) {
  details->entry = GetActiveEntry();
  NotificationDetails notification_details =
      Details<LoadCommittedDetails>(details);

  ssl_manager_.DidCommitProvisionalLoad(notification_details);

  tab_contents_->NotifyNavigationStateChanged(
      kInvalidateAllButShelves | extra_invalidate_flags);

  NotificationService::current()->Notify(
      NotificationType::NAV_ENTRY_COMMITTED,
      Source<NavigationController>(this),
      notification_details);
}
