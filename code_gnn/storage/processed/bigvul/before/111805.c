void SyncBackendHost::HandleSyncCycleCompletedOnFrontendLoop(
    const SyncSessionSnapshot& snapshot) {
  if (!frontend_)
    return;
  DCHECK_EQ(MessageLoop::current(), frontend_loop_);

  last_snapshot_ = snapshot;

  SDVLOG(1) << "Got snapshot " << snapshot.ToString();

  const syncable::ModelTypeSet to_migrate =
      snapshot.syncer_status().types_needing_local_migration;
  if (!to_migrate.Empty())
    frontend_->OnMigrationNeededForTypes(to_migrate);

  if (initialized())
    AddExperimentalTypes();

  if (pending_download_state_.get()) {
    const syncable::ModelTypeSet types_to_add =
        pending_download_state_->types_to_add;
    const syncable::ModelTypeSet added_types =
        pending_download_state_->added_types;
    DCHECK(types_to_add.HasAll(added_types));
    const syncable::ModelTypeSet initial_sync_ended =
        snapshot.initial_sync_ended();
    const syncable::ModelTypeSet failed_configuration_types =
        Difference(added_types, initial_sync_ended);
    SDVLOG(1)
        << "Added types: "
        << syncable::ModelTypeSetToString(added_types)
        << ", configured types: "
        << syncable::ModelTypeSetToString(initial_sync_ended)
        << ", failed configuration types: "
        << syncable::ModelTypeSetToString(failed_configuration_types);

    if (!failed_configuration_types.Empty() &&
        snapshot.retry_scheduled()) {
      if (!pending_download_state_->retry_in_progress) {
        pending_download_state_->retry_callback.Run();
        pending_download_state_->retry_in_progress = true;
      }
      return;
    }

    scoped_ptr<PendingConfigureDataTypesState> state(
        pending_download_state_.release());
    state->ready_task.Run(failed_configuration_types);

    if (!failed_configuration_types.Empty())
      return;
  }

  if (initialized())
    frontend_->OnSyncCycleCompleted();
}
