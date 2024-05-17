bool SyncBackendHost::IsUsingExplicitPassphrase() {
  return IsNigoriEnabled() &&
      core_->sync_manager()->IsUsingExplicitPassphrase();
}
