void SyncBackendHost::LogUnsyncedItems(int level) const {
  DCHECK(initialized());
  return core_->sync_manager()->LogUnsyncedItems(level);
}
