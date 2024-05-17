void SyncBackendHost::Core::DoStopSyncManagerForShutdown(
    const base::Closure& closure) {
  DCHECK(sync_manager_.get());
  sync_manager_->StopSyncingForShutdown(closure);
}
