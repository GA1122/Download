bool SyncBackendHost::IsUsingExplicitPassphrase() {
  return initialized() && IsNigoriEnabled() &&
      core_->sync_manager()->InitialSyncEndedForAllEnabledTypes() &&
      core_->sync_manager()->IsUsingExplicitPassphrase();
}
