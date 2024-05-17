void SyncBackendHost::ConfigureDataTypes(
    const syncable::ModelTypeSet& types_to_add,
    const syncable::ModelTypeSet& types_to_remove,
    sync_api::ConfigureReason reason,
    base::Callback<void(bool)> ready_task,
    bool enable_nigori) {
  syncable::ModelTypeSet types_to_add_with_nigori = types_to_add;
  syncable::ModelTypeSet types_to_remove_with_nigori = types_to_remove;
  if (enable_nigori) {
    types_to_add_with_nigori.insert(syncable::NIGORI);
    types_to_remove_with_nigori.erase(syncable::NIGORI);
  } else {
    types_to_add_with_nigori.erase(syncable::NIGORI);
    types_to_remove_with_nigori.insert(syncable::NIGORI);
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

  if (!types_to_remove_with_nigori.empty()) {
    sync_thread_.message_loop()->PostTask(
        FROM_HERE,
        NewRunnableMethod(
            core_.get(),
            &SyncBackendHost::Core::DoRequestCleanupDisabledTypes));
  }

  StartConfiguration(NewCallback(core_.get(),
      &SyncBackendHost::Core::FinishConfigureDataTypes));
}
