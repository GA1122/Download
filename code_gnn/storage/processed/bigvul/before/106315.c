void SyncBackendHost::FinishConfigureDataTypesOnFrontendLoop() {
  DCHECK_EQ(MessageLoop::current(), frontend_loop_);

  VLOG(1) << "Syncer in config mode. SBH executing"
          << "FinishConfigureDataTypesOnFrontendLoop";
  if (pending_config_mode_state_->deleted_type) {
    core_thread_.message_loop()->PostTask(FROM_HERE,
        NewRunnableMethod(core_.get(),
        &SyncBackendHost::Core::DeferNudgeForCleanup));
  }

  if (pending_config_mode_state_->added_types.none() &&
      !core_->syncapi()->InitialSyncEndedForAllEnabledTypes()) {
    LOG(WARNING) << "No new types, but initial sync not finished."
                 << "Possible sync db corruption / removal.";
    pending_config_mode_state_->added_types =
        syncable::ModelTypeBitSetFromSet(
            pending_config_mode_state_->initial_types);
  }

  if (pending_config_mode_state_->added_types.none()) {
    VLOG(1) << "SyncBackendHost(" << this << "): No new types added. "
            << "Calling ready_task directly";
    pending_config_mode_state_->ready_task->Run();
  } else {
    pending_download_state_.reset(pending_config_mode_state_.release());

    syncable::ModelTypeBitSet types_copy(pending_download_state_->added_types);
    if (IsNigoriEnabled())
      types_copy.set(syncable::NIGORI);
    VLOG(1) <<  "SyncBackendHost(" << this << "):New Types added. "
            << "Calling DoRequestConfig";
    core_thread_.message_loop()->PostTask(FROM_HERE,
         NewRunnableMethod(core_.get(),
                           &SyncBackendHost::Core::DoRequestConfig,
                           types_copy,
                           pending_download_state_->reason));
  }

  pending_config_mode_state_.reset();

  core_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(core_.get(),
                        &SyncBackendHost::Core::DoUpdateEnabledTypes));
}
