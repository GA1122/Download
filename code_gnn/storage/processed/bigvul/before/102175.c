void SyncManager::RequestCleanupDisabledTypes() {
  if (data_->scheduler())
    data_->scheduler()->ScheduleCleanupDisabledTypes();
}
