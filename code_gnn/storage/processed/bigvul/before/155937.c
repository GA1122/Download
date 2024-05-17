void PeopleHandler::HandleShowSetupUI(const base::ListValue* args) {
  AllowJavascript();

  ProfileSyncService* service = GetSyncService();

  if (unified_consent::IsUnifiedConsentFeatureEnabled()) {
    if (service && !sync_blocker_)
      sync_blocker_ = service->GetSetupInProgressHandle();

    GetLoginUIService()->SetLoginUI(this);

    PushSyncPrefs();
    return;
  }

  if (!service) {
    CloseUI();
    return;
  }

  if (!SigninManagerFactory::GetForProfile(profile_)->IsAuthenticated()) {
    DLOG(WARNING) << "Cannot display sync setup UI when not signed in";
    CloseUI();
    return;
  }

  GetLoginUIService()->SetLoginUI(this);

  if (!sync_blocker_)
    sync_blocker_ = service->GetSetupInProgressHandle();

  if (sync_startup_tracker_)
    return;

  if (!service->IsEngineInitialized() ||
      service->HasDisableReason(
          syncer::SyncService::DISABLE_REASON_USER_CHOICE)) {
    sync_startup_tracker_ =
        std::make_unique<SyncStartupTracker>(profile_, this);
    service->RequestStart();

    if (SyncStartupTracker::GetSyncServiceState(profile_) !=
        SyncStartupTracker::SYNC_STARTUP_ERROR) {
      DisplaySpinner();
    }
    return;
  }

  PushSyncPrefs();
}
