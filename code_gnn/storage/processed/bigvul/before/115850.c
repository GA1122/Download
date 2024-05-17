void NavigationController::NotifyNavigationEntryCommitted(
    content::LoadCommittedDetails* details) {
  details->entry = GetActiveEntry();
  content::NotificationDetails notification_details =
      content::Details<content::LoadCommittedDetails>(details);

  ssl_manager_.DidCommitProvisionalLoad(notification_details);

  tab_contents_->NotifyNavigationStateChanged(kInvalidateAll);

  content::NotificationService::current()->Notify(
      content::NOTIFICATION_NAV_ENTRY_COMMITTED,
      content::Source<NavigationController>(this),
      notification_details);
}
