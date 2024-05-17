void SyncBackendHost::Core::HandleSyncCycleCompletedOnFrontendLoop(
    SyncSessionSnapshot* snapshot) {
  if (!host_ || !host_->frontend_)
    return;
  DCHECK_EQ(MessageLoop::current(), host_->frontend_loop_);

  host_->last_snapshot_.reset(snapshot);

  SVLOG(1) << "Got snapshot " << snapshot->ToString();

  const syncable::ModelTypeSet& to_migrate =
      snapshot->syncer_status.types_needing_local_migration;
  if (!to_migrate.empty())
    host_->frontend_->OnMigrationNeededForTypes(to_migrate);

  syncable::ModelTypeSet to_add;
  if (host_->initialized() &&
      sync_manager()->ReceivedExperimentalTypes(&to_add)) {
    host_->frontend_->OnDataTypesChanged(to_add);
  }

  if (host_->pending_download_state_.get()) {
    scoped_ptr<PendingConfigureDataTypesState> state(
        host_->pending_download_state_.release());
    DCHECK(
        std::includes(state->types_to_add.begin(), state->types_to_add.end(),
                      state->added_types.begin(), state->added_types.end()));
    SVLOG(1)
        << "Added types: "
        << syncable::ModelTypeSetToString(state->added_types)
        << ", configured types: "
        << syncable::ModelTypeBitSetToString(snapshot->initial_sync_ended);
    syncable::ModelTypeBitSet added_types =
        syncable::ModelTypeBitSetFromSet(state->added_types);
    bool found_all_added =
        (added_types & snapshot->initial_sync_ended) == added_types;
    state->ready_task.Run(found_all_added);
    if (!found_all_added)
      return;
  }

  if (host_->initialized())
    host_->frontend_->OnSyncCycleCompleted();
}
