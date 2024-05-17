void SyncManager::StopSyncingForShutdown(const base::Closure& callback) {
  data_->StopSyncingForShutdown(callback);
}
