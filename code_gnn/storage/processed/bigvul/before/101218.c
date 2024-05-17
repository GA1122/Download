Syncer::ScopedSyncStartStopTracker::ScopedSyncStartStopTracker(
    sessions::SyncSession* session) : session_(session) {
  session_->status_controller()->SetSyncInProgressAndUpdateStartTime(true);
}
