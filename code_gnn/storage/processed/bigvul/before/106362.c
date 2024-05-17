void SyncBackendHost::StartSyncingWithServer() {
  VLOG(1) << "SyncBackendHost::StartSyncingWithServer called.";
  core_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(core_.get(), &SyncBackendHost::Core::DoStartSyncing));
}
