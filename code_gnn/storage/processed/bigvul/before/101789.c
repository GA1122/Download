void Browser::RestoreTab() {
  UserMetrics::RecordAction(UserMetricsAction("RestoreTab"));
  TabRestoreService* service =
      TabRestoreServiceFactory::GetForProfile(profile_);
  if (!service)
    return;

  service->RestoreMostRecentEntry(tab_restore_service_delegate());
}
