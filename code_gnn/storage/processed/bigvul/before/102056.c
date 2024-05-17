void SyncBackendHost::FinishConfigureDataTypesOnFrontendLoop() {
  DCHECK_EQ(MessageLoop::current(), frontend_loop_);

  SVLOG(1) << "Syncer in config mode. SBH executing "
           << "FinishConfigureDataTypesOnFrontendLoop";

  if (pending_config_mode_state_->added_types.empty() &&
      !core_->sync_manager()->InitialSyncEndedForAllEnabledTypes()) {
    SLOG(WARNING) << "No new types, but initial sync not finished."
                  << "Possible sync db corruption / removal.";
    pending_config_mode_state_->added_types =
        pending_config_mode_state_->types_to_add;
  }

  if (pending_config_mode_state_->added_types.empty()) {
    SVLOG(1) << "No new types added; calling ready_task directly";
    pending_config_mode_state_->ready_task.Run(true);
  } else {
    pending_download_state_.reset(pending_config_mode_state_.release());

    syncable::ModelTypeSet types_to_config =
        pending_download_state_->added_types;
    if (IsNigoriEnabled()) {
      types_to_config.insert(syncable::NIGORI);
    }
    SVLOG(1) << "Types " << ModelTypeSetToString(types_to_config)
            << " added; calling DoRequestConfig";
    sync_thread_.message_loop()->PostTask(FROM_HERE,
         NewRunnableMethod(core_.get(),
                           &SyncBackendHost::Core::DoRequestConfig,
                           syncable::ModelTypeBitSetFromSet(types_to_config),
                           pending_download_state_->reason));
  }

  pending_config_mode_state_.reset();

  sync_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(core_.get(),
                        &SyncBackendHost::Core::DoUpdateEnabledTypes));
}
