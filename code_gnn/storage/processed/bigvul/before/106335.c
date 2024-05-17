bool SyncBackendHost::IsUsingExplicitPassphrase() {
  return IsNigoriEnabled() && syncapi_initialized_ &&
      core_->syncapi()->InitialSyncEndedForAllEnabledTypes() &&
      core_->syncapi()->IsUsingExplicitPassphrase();
}
