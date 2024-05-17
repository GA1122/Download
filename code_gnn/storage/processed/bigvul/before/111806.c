void SyncBackendHost::InitCore(const DoInitializeOptions& options) {
  sync_thread_.message_loop()->PostTask(FROM_HERE,
      base::Bind(&SyncBackendHost::Core::DoInitialize, core_.get(), options));
}
