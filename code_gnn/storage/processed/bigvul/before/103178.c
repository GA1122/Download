void Browser::RestoreTab() {
  UserMetrics::RecordAction(UserMetricsAction("RestoreTab"), profile_);
  TabRestoreService* service = profile_->GetTabRestoreService();
  if (!service)
    return;

  service->RestoreMostRecentEntry(tab_restore_service_delegate());
}
