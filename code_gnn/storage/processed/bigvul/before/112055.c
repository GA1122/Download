  bool SyncShareAsDelegate(
      SyncScheduler::SyncSessionJob::SyncSessionJobPurpose purpose) {
    SyncerStep start;
    SyncerStep end;
    SyncScheduler::SetSyncerStepsForPurpose(purpose, &start, &end);

    session_.reset(MakeSession());
    syncer_->SyncShare(session_.get(), start, end);
    return session_->HasMoreToSync();
  }
