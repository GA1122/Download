void SyncBackendHost::ConfigureDataTypes(
    const DataTypeController::TypeMap& data_type_controllers,
    const syncable::ModelTypeSet& types,
    sync_api::ConfigureReason reason,
    CancelableTask* ready_task) {
  DCHECK(!pending_config_mode_state_.get());
  DCHECK(!pending_download_state_.get());
  DCHECK(syncapi_initialized_);

  if (types.count(syncable::AUTOFILL_PROFILE) != 0) {
    ConfigureAutofillMigration();
  }

  {
    base::AutoLock lock(registrar_lock_);
    pending_config_mode_state_.reset(
        MakePendingConfigModeState(data_type_controllers, types, ready_task,
                                   &registrar_.routing_info, reason));
  }

  StartConfiguration(NewCallback(core_.get(),
      &SyncBackendHost::Core::FinishConfigureDataTypes));
}
