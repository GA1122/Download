void OneClickSigninSyncStarter::Initialize(Profile* profile, Browser* browser) {
  DCHECK(profile);
  profile_ = profile;
  browser_ = browser;

  if (browser)
    desktop_type_ = browser->host_desktop_type();

  signin_tracker_.reset(new SigninTracker(profile_, this));

  ProfileSyncService* profile_sync_service = GetProfileSyncService();
  if (profile_sync_service)
    profile_sync_service->SetSetupInProgress(true);

  browser_sync::SyncPrefs sync_prefs(profile_->GetPrefs());
  sync_prefs.SetStartSuppressed(false);
}
