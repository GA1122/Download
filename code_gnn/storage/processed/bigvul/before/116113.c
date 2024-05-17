void SyncManager::UpdateEnabledTypes() {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->UpdateEnabledTypes();
}
