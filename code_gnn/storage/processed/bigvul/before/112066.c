void SyncManager::AddObserver(Observer* observer) {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->AddObserver(observer);
}
