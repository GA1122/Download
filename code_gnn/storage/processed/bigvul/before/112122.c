void SyncManager::StartSyncingNormally() {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->StartSyncingNormally();
}
