bool ChromePasswordManagerClient::ShouldAnnotateNavigationEntries(
    Profile* profile) {
  if (!ChromeMetricsServiceAccessor::IsMetricsAndCrashReportingEnabled())
    return false;

  browser_sync::ProfileSyncService* profile_sync_service =
      ProfileSyncServiceFactory::GetForProfile(profile);
  if (!profile_sync_service || !profile_sync_service->IsSyncFeatureActive() ||
      profile_sync_service->GetUserSettings()->IsUsingSecondaryPassphrase()) {
    return false;
  }

  return true;
}
