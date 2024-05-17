void SyncBackendHost::Core::DoStartConfiguration(Callback0::Type* callback) {
  sync_manager_->StartConfigurationMode(callback);
}
