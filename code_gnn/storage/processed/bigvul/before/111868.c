void ProfileSyncService::CreateBackend() {
  backend_.reset(
      new SyncBackendHost(profile_->GetDebugName(),
                          profile_, sync_prefs_.AsWeakPtr()));
}
