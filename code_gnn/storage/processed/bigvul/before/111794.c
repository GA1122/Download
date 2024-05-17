void SyncBackendHost::EnableEncryptEverything() {
  sync_thread_.message_loop()->PostTask(FROM_HERE,
     base::Bind(&SyncBackendHost::Core::DoEnableEncryptEverything,
                core_.get()));
}
