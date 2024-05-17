void SyncManager::RequestCleanupDisabledTypes() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (data_->scheduler())
    data_->scheduler()->ScheduleCleanupDisabledTypes();
}
