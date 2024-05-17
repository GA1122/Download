bool SyncBackendHost::RequestClearServerData() {
  core_thread_.message_loop()->PostTask(FROM_HERE,
     NewRunnableMethod(core_.get(),
     &SyncBackendHost::Core::DoRequestClearServerData));
  return true;
}
