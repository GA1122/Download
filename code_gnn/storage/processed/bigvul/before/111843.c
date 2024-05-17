void SyncBackendHost::StartSyncingWithServer() {
  SDVLOG(1) << "SyncBackendHost::StartSyncingWithServer called.";
  sync_thread_.message_loop()->PostTask(FROM_HERE,
      base::Bind(&SyncBackendHost::Core::DoStartSyncing, core_.get()));
}
