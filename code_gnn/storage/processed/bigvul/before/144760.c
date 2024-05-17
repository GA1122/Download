    OnNonPersistentNotificationCreated(
        content::WebContents* contents,
        const PageNavigationIdentity& page_navigation_id) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (web_contents() != contents)
    return;

  MaybeNotifyBackgroundFeatureUsage(
      &SiteCharacteristicsDataWriter::NotifyUsesNotificationsInBackground);
}
