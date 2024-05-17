void SyncManager::SyncInternal::AddObserver(
    SyncManager::Observer* observer) {
  base::AutoLock lock(observers_lock_);
  observers_.AddObserver(observer);
}
