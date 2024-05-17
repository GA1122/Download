void SyncManager::UpdateCredentials(const SyncCredentials& credentials) {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->UpdateCredentials(credentials);
}
