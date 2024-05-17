void SyncManager::SyncInternal::RequestEarlyExit() {
  if (scheduler()) {
    scheduler()->RequestEarlyExit();
  }

  if (connection_manager_.get()) {
    connection_manager_->TerminateAllIO();
  }
}
