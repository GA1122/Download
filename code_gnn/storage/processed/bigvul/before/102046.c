void SyncBackendHost::Core::DoRequestConfig(
    const syncable::ModelTypeBitSet& types_to_config,
    sync_api::ConfigureReason reason) {
  sync_manager_->RequestConfig(types_to_config, reason);
}
