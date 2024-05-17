void SyncBackendHost::Core::DoUpdateEnabledTypes() {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->UpdateEnabledTypes();
}
