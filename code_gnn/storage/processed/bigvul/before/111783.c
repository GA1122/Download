void SyncBackendHost::Core::DoRequestCleanupDisabledTypes() {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->RequestCleanupDisabledTypes();
}
