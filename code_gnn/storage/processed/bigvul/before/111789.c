void SyncBackendHost::Core::DoStartConfiguration(
    const base::Closure& callback) {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->StartConfigurationMode(callback);
}
