void SyncBackendHost::Core::DoUpdateCredentials(
    const SyncCredentials& credentials) {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->UpdateCredentials(credentials);
}
