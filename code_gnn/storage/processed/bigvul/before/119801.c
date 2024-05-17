void NavigationControllerImpl::NotifyNavigationEntryCommitted(
    LoadCommittedDetails* details) {
  details->entry = GetActiveEntry();

  ssl_manager_.DidCommitProvisionalLoad(*details);

  web_contents_->NotifyNavigationStateChanged(kInvalidateAll);
  web_contents_->NotifyNavigationEntryCommitted(*details);

  NotificationDetails notification_details =
      Details<LoadCommittedDetails>(details);
  NotificationService::current()->Notify(
      NOTIFICATION_NAV_ENTRY_COMMITTED,
      Source<NavigationController>(this),
      notification_details);
}
