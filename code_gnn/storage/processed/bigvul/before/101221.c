Syncer::ScopedSyncStartStopTracker::~ScopedSyncStartStopTracker() {
  session_->status_controller()->SetSyncInProgressAndUpdateStartTime(false);
}
