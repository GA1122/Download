void SyncBackendHost::Core::DoUpdateCredentials(
    const SyncCredentials& credentials) {
  DCHECK(MessageLoop::current() == host_->sync_thread_.message_loop());
  sync_manager_->UpdateCredentials(credentials);
}
