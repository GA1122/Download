void SyncManager::SaveChanges() {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->SaveChanges();
}
