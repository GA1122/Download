void SyncBackendHost::Core::DoUpdateCredentials(
    const SyncCredentials& credentials) {
  DCHECK(MessageLoop::current() == host_->core_thread_.message_loop());
  syncapi_->UpdateCredentials(credentials);
}
