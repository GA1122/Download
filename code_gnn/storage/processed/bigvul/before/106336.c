void SyncBackendHost::LogUnsyncedItems(int level) const {
  DCHECK(syncapi_initialized_);
  return core_->syncapi()->LogUnsyncedItems(level);
}
