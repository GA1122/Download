void SyncBackendHost::Core::ConnectChildJsEventRouter() {
  DCHECK_EQ(MessageLoop::current(), host_->core_thread_.message_loop());
  if (!syncapi_->GetJsBackend()->GetParentJsEventRouter()) {
    syncapi_->GetJsBackend()->SetParentJsEventRouter(this);
    syncapi_->AddObserver(&sync_manager_observer_);
  }
}
