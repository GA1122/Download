void SyncBackendHost::Core::DoStartSyncing() {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->StartSyncingNormally();
}
