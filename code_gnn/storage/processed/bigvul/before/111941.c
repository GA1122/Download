void ProfileSyncService::StartUp() {
  if (backend_.get()) {
    DVLOG(1) << "Skipping bringing up backend host.";
    return;
  }

  DCHECK(IsSyncEnabledAndLoggedIn());

  last_synced_time_ = sync_prefs_.GetLastSyncedTime();

#if defined(OS_CHROMEOS)
  std::string bootstrap_token = sync_prefs_.GetEncryptionBootstrapToken();
  if (bootstrap_token.empty()) {
    sync_prefs_.SetEncryptionBootstrapToken(
        sync_prefs_.GetSpareBootstrapToken());
  }
#endif
  CreateBackend();

  InitializeBackend(!HasSyncSetupCompleted());

  if (!sync_global_error_.get()) {
    sync_global_error_.reset(new SyncGlobalError(this, signin()));
    GlobalErrorServiceFactory::GetForProfile(profile_)->AddGlobalError(
        sync_global_error_.get());
    AddObserver(sync_global_error_.get());
  }
}
