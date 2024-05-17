void SyncManager::StartConfigurationMode(const base::Closure& callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (!data_->scheduler()) {
    LOG(INFO)
        << "SyncManager::StartConfigurationMode: could not start "
        << "configuration mode because because scheduler is null";
    return;
  }
  data_->scheduler()->Start(
      browser_sync::SyncScheduler::CONFIGURATION_MODE, callback);
}
