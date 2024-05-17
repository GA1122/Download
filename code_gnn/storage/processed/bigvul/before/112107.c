void SyncManager::RequestClearServerData() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (data_->scheduler())
    data_->scheduler()->ScheduleClearUserData();
}
