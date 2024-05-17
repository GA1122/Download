void SyncBackendHost::Core::DoUpdateEnabledTypes() {
  DCHECK(MessageLoop::current() == host_->core_thread_.message_loop());
  syncapi_->UpdateEnabledTypes();
}
