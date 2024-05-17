void SyncBackendHost::InitCore(const Core::DoInitializeOptions& options) {
  sync_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(core_.get(), &SyncBackendHost::Core::DoInitialize,
                        options));
}
