void SyncBackendHost::StartConfiguration(Callback0::Type* callback) {
  sync_thread_.message_loop()->PostTask(FROM_HERE, NewRunnableMethod(
      core_.get(), &SyncBackendHost::Core::DoStartConfiguration, callback));
}
