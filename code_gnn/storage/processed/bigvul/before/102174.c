void SyncManager::SyncInternal::RemoveObserver(
    SyncManager::Observer* observer) {
  base::AutoLock lock(observers_lock_);
  observers_.RemoveObserver(observer);
}
