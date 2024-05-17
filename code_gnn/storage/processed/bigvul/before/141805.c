bool ChromeMetricsServiceClient::RegisterForProfileEvents(Profile* profile) {
#if defined(OS_CHROMEOS)
  if (chromeos::ProfileHelper::IsSigninProfile(profile) ||
      chromeos::ProfileHelper::IsLockScreenAppProfile(profile)) {
    return true;
  }
#endif
#if defined(OS_WIN) || defined(OS_MACOSX) || \
    (defined(OS_LINUX) && !defined(OS_CHROMEOS))
  metrics::DesktopProfileSessionDurationsServiceFactory::GetForBrowserContext(
      profile);
#endif

  history::HistoryService* history_service =
      HistoryServiceFactory::GetForProfile(profile,
                                           ServiceAccessType::IMPLICIT_ACCESS);
  if (!history_service) {
    return false;
  }

  ObserveServiceForDeletions(history_service);

  syncer::SyncService* sync =
      ProfileSyncServiceFactory::GetSyncServiceForBrowserContext(profile);
  if (!sync) {
    return false;
  }
  ObserveServiceForSyncDisables(sync, profile->GetPrefs());
  return true;
}
