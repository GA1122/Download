void ProfileSyncService::Initialize() {
  InitSettings();

  ClearStaleErrors();

  sync_prefs_.AddSyncPrefObserver(this);

  if (IsManaged()) {
    DisableForUser();
    return;
  }

  RegisterAuthNotifications();

  if (!HasSyncSetupCompleted() || signin_->GetAuthenticatedUsername().empty()) {
    DisableForUser();
  }

  TryStart();
}
