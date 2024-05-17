SyncManager::~SyncManager() {
  DCHECK(thread_checker_.CalledOnValidThread());
  delete data_;
}
