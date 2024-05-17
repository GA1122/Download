bool ProfileSyncService::IsUsingSecondaryPassphrase() const {
  return backend_->IsUsingExplicitPassphrase();
}
