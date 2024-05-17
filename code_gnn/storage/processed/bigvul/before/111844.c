void SyncBackendHost::StopSyncManagerForShutdown(
    const base::Closure& closure) {
  DCHECK_GT(initialization_state_, NOT_ATTEMPTED);
  if (initialization_state_ == CREATING_SYNC_MANAGER) {
    DCHECK(sync_thread_.IsRunning());
    sync_thread_.message_loop()->PostTask(FROM_HERE,
        base::Bind(
            &SyncBackendHost::Core::DoStopSyncManagerForShutdown,
            core_.get(),
            closure));
  } else {
    core_->DoStopSyncManagerForShutdown(closure);
  }
}
