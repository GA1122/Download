void SyncBackendHost::Core::HandleSyncCycleCompletedOnFrontendLoop(
    SyncSessionSnapshot* snapshot) {
  if (!host_ || !host_->frontend_)
    return;
  DCHECK_EQ(MessageLoop::current(), host_->frontend_loop_);

  host_->last_snapshot_.reset(snapshot);

  const syncable::ModelTypeSet& to_migrate =
      snapshot->syncer_status.types_needing_local_migration;
  if (!to_migrate.empty())
    host_->frontend_->OnMigrationNeededForTypes(to_migrate);

  if (host_->pending_download_state_.get()) {
    bool found_all_added = true;
    for (syncable::ModelTypeSet::const_iterator it =
             host_->pending_download_state_->initial_types.begin();
         it != host_->pending_download_state_->initial_types.end();
         ++it) {
      if (host_->pending_download_state_->added_types.test(*it))
        found_all_added &= snapshot->initial_sync_ended.test(*it);
    }
    if (!found_all_added) {
      NOTREACHED() << "Update didn't return updates for all types requested.";
    } else {
      host_->pending_download_state_->ready_task->Run();
    }
    host_->pending_download_state_.reset();
  }
  host_->frontend_->OnSyncCycleCompleted();
}
