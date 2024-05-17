void SyncManager::SyncInternal::StopSyncingForShutdown(
    const base::Closure& callback) {
  DVLOG(2) << "StopSyncingForShutdown";
  if (scheduler())   
    scheduler()->RequestStop(callback);
  else
    created_on_loop_->PostTask(FROM_HERE, callback);

  if (connection_manager_.get())
    connection_manager_->TerminateAllIO();
}
