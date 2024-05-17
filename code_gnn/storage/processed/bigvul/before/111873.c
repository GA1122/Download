bool ProfileSyncService::FirstSetupInProgress() const {
  return !HasSyncSetupCompleted() && setup_in_progress_;
}
