void SyncBackendHost::StartConfiguration(const base::Closure& callback) {
  sync_thread_.message_loop()->PostTask(FROM_HERE, base::Bind(
      &SyncBackendHost::Core::DoStartConfiguration, core_.get(), callback));
}
