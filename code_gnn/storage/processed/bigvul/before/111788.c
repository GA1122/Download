void SyncBackendHost::Core::DoShutdown(bool sync_disabled) {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  if (!sync_manager_.get())
    return;

  save_changes_timer_.reset();
  sync_manager_->ShutdownOnSyncThread();
  sync_manager_->RemoveObserver(this);
  sync_manager_.reset();
  registrar_ = NULL;

  if (sync_disabled)
    DeleteSyncDataFolder();

  sync_loop_ = NULL;

  host_.Reset();
}
