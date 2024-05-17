void SyncManager::RemoveObserver(Observer* observer) {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->RemoveObserver(observer);
}
