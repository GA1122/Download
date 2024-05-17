void SyncBackendHost::Core::DisconnectChildJsEventRouter() {
  DCHECK_EQ(MessageLoop::current(), host_->core_thread_.message_loop());
  syncapi_->GetJsBackend()->RemoveParentJsEventRouter();
  syncapi_->RemoveObserver(&sync_manager_observer_);
}
