void SyncBackendHost::ConfigureDataTypes(
    sync_api::ConfigureReason reason,
    syncable::ModelTypeSet types_to_add,
    syncable::ModelTypeSet types_to_remove,
    NigoriState nigori_state,
    base::Callback<void(syncable::ModelTypeSet)> ready_task,
    base::Callback<void()> retry_callback) {
  syncable::ModelTypeSet types_to_add_with_nigori = types_to_add;
  syncable::ModelTypeSet types_to_remove_with_nigori = types_to_remove;
  if (nigori_state == WITH_NIGORI) {
    types_to_add_with_nigori.Put(syncable::NIGORI);
    types_to_remove_with_nigori.Remove(syncable::NIGORI);
  } else {
    types_to_add_with_nigori.Remove(syncable::NIGORI);
    types_to_remove_with_nigori.Put(syncable::NIGORI);
  }
  DCHECK(!pending_config_mode_state_.get());
  DCHECK(!pending_download_state_.get());
  DCHECK_GT(initialization_state_, NOT_INITIALIZED);

  pending_config_mode_state_.reset(new PendingConfigureDataTypesState());
  pending_config_mode_state_->ready_task = ready_task;
  pending_config_mode_state_->types_to_add = types_to_add_with_nigori;
  pending_config_mode_state_->added_types =
      registrar_->ConfigureDataTypes(types_to_add_with_nigori,
                                     types_to_remove_with_nigori);
  pending_config_mode_state_->reason = reason;
  pending_config_mode_state_->retry_callback = retry_callback;

  if (!types_to_remove_with_nigori.Empty()) {
    sync_thread_.message_loop()->PostTask(
        FROM_HERE,
        base::Bind(&SyncBackendHost::Core::DoRequestCleanupDisabledTypes,
                   core_.get()));
  }

  StartConfiguration(
      base::Bind(&SyncBackendHost::Core::FinishConfigureDataTypes,
                 core_.get()));
}
