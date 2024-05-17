void SyncBackendHost::Core::DoStartConfiguration(Callback0::Type* callback) {
  syncapi_->StartConfigurationMode(callback);
}
