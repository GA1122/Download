void SyncManager::ShutdownOnSyncThread() {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->ShutdownOnSyncThread();
}
