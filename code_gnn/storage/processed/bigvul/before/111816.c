void SyncBackendHost::NotifyUpdatedToken(const std::string& token) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  TokenAvailableDetails details(GaiaConstants::kSyncService, token);
  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_TOKEN_UPDATED,
      content::Source<Profile>(profile_),
      content::Details<const TokenAvailableDetails>(&details));
}
