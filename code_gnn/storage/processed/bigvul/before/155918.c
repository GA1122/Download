void PeopleHandler::CloseSyncSetup() {
  engine_start_timer_.reset();

  sync_startup_tracker_.reset();

  ProfileSyncService* sync_service = GetSyncService();

  LoginUIService* service = GetLoginUIService();
  if (service) {
    if ((service->current_login_ui() == this) &&
        (!sync_service || (!sync_service->IsFirstSetupComplete() &&
                           sync_service->GetAuthError().state() ==
                               GoogleServiceAuthError::NONE))) {
      if (configuring_sync_) {
        ProfileSyncService::SyncEvent(
            ProfileSyncService::CANCEL_DURING_CONFIGURE);

        if (sync_service) {
          DVLOG(1) << "Sync setup aborted by user action";
          sync_service->RequestStop(ProfileSyncService::CLEAR_DATA);
#if !defined(OS_CHROMEOS)
          if (sync_service->IsFirstSetupInProgress()) {
            SigninManagerFactory::GetForProfile(profile_)
                ->SignOut(signin_metrics::ABORT_SIGNIN,
                          signin_metrics::SignoutDelete::IGNORE_METRIC);
          }
#endif
        }
      }
    }

    service->LoginUIClosed(this);
  }

  sync_blocker_.reset();

  configuring_sync_ = false;
}
