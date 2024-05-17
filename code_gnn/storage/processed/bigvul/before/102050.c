void SyncBackendHost::Core::DoStartSyncing() {
  DCHECK(MessageLoop::current() == host_->sync_thread_.message_loop());
  sync_manager_->StartSyncingNormally();
}
