void SyncBackendHost::Core::DoUpdateEnabledTypes() {
  DCHECK(MessageLoop::current() == host_->sync_thread_.message_loop());
  sync_manager_->UpdateEnabledTypes();
}
