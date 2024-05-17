bool SyncManager::SyncInternal::HaveObservers() const {
  base::AutoLock lock(observers_lock_);
  return observers_.size() > 0;
}
