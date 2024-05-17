BrowserCommandController::BrowserCommandController(Browser* browser)
    : browser_(browser),
      ALLOW_THIS_IN_INITIALIZER_LIST(command_updater_(this)),
      block_command_execution_(false),
      last_blocked_command_id_(-1),
      last_blocked_command_disposition_(CURRENT_TAB) {
  browser_->tab_strip_model()->AddObserver(this);
  PrefService* local_state = g_browser_process->local_state();
  if (local_state) {
    local_pref_registrar_.Init(local_state);
    local_pref_registrar_.Add(prefs::kAllowFileSelectionDialogs, this);
    local_pref_registrar_.Add(prefs::kInManagedMode, this);
  }

  profile_pref_registrar_.Init(profile()->GetPrefs());
  profile_pref_registrar_.Add(prefs::kDevToolsDisabled, this);
  profile_pref_registrar_.Add(prefs::kEditBookmarksEnabled, this);
  profile_pref_registrar_.Add(prefs::kShowBookmarkBar, this);
  profile_pref_registrar_.Add(prefs::kIncognitoModeAvailability, this);
  profile_pref_registrar_.Add(prefs::kPrintingEnabled, this);

  InitCommandState();

  TabRestoreService* tab_restore_service =
      TabRestoreServiceFactory::GetForProfile(profile());
  if (tab_restore_service) {
    tab_restore_service->AddObserver(this);
    TabRestoreServiceChanged(tab_restore_service);
  }

  ProfileSyncService* service =
      ProfileSyncServiceFactory::GetInstance()->GetForProfile(profile());
  if (service)
    service->AddObserver(this);
}
