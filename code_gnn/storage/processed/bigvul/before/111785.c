void SyncBackendHost::Core::DoRequestConfig(
    syncable::ModelTypeSet types_to_config,
    sync_api::ConfigureReason reason) {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->RequestConfig(types_to_config, reason);
}
