Browser::Browser(Type type, Profile* profile)
    : type_(type),
      profile_(profile),
      window_(NULL),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          tab_handler_(TabHandler::CreateTabHandler(this))),
      command_updater_(this),
      toolbar_model_(this),
      chrome_updater_factory_(this),
      is_attempting_to_close_browser_(false),
      cancel_download_confirmation_state_(NOT_PROMPTED),
      show_state_(ui::SHOW_STATE_DEFAULT),
      is_session_restore_(false),
      method_factory_(this),
      block_command_execution_(false),
      last_blocked_command_id_(-1),
      last_blocked_command_disposition_(CURRENT_TAB),
      pending_web_app_action_(NONE),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          tab_restore_service_delegate_(
              new BrowserTabRestoreServiceDelegate(this))),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          synced_window_delegate_(
              new BrowserSyncedWindowDelegate(this))),
      bookmark_bar_state_(BookmarkBar::HIDDEN),
      fullscreened_tab_(NULL),
      tab_caused_fullscreen_(false) {
  registrar_.Add(this, content::NOTIFICATION_SSL_VISIBLE_STATE_CHANGED,
                 NotificationService::AllSources());
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UPDATE_DISABLED,
                 Source<Profile>(profile_));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_LOADED,
                 Source<Profile>(profile_));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNLOADED,
                 Source<Profile>(profile_));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNINSTALLED,
                 Source<Profile>(profile_));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_PROCESS_TERMINATED,
                 NotificationService::AllSources());
  registrar_.Add(
      this, chrome::NOTIFICATION_BROWSER_THEME_CHANGED,
      Source<ThemeService>(ThemeServiceFactory::GetForProfile(profile_)));
  registrar_.Add(this, chrome::NOTIFICATION_TAB_CONTENT_SETTINGS_CHANGED,
                 NotificationService::AllSources());
  registrar_.Add(this,
                 chrome::NOTIFICATION_BOOKMARK_BAR_VISIBILITY_PREF_CHANGED,
                 NotificationService::AllBrowserContextsAndSources());

  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_READY_FOR_INSTALL,
                 NotificationService::AllSources());

  PrefService* local_state = g_browser_process->local_state();
  if (local_state) {
    local_pref_registrar_.Init(local_state);
    local_pref_registrar_.Add(prefs::kPrintingEnabled, this);
    local_pref_registrar_.Add(prefs::kAllowFileSelectionDialogs, this);
    local_pref_registrar_.Add(prefs::kMetricsReportingEnabled, this);
  }

  profile_pref_registrar_.Init(profile_->GetPrefs());
  profile_pref_registrar_.Add(prefs::kDevToolsDisabled, this);
  profile_pref_registrar_.Add(prefs::kEditBookmarksEnabled, this);
  profile_pref_registrar_.Add(prefs::kEnableBookmarkBar, this);
  profile_pref_registrar_.Add(prefs::kHomePage, this);
  profile_pref_registrar_.Add(prefs::kInstantEnabled, this);
  profile_pref_registrar_.Add(prefs::kIncognitoModeAvailability, this);
  profile_pref_registrar_.Add(prefs::kSearchSuggestEnabled, this);

  InitCommandState();
  BrowserList::AddBrowser(this);

  encoding_auto_detect_.Init(prefs::kWebKitUsesUniversalDetector,
                             profile_->GetPrefs(), NULL);
  use_vertical_tabs_.Init(prefs::kUseVerticalTabs, profile_->GetPrefs(), this);
  use_compact_navigation_bar_.Init(prefs::kUseCompactNavigationBar,
                                   profile_->GetPrefs(),
                                   this);

  if (!TabMenuModel::AreVerticalTabsEnabled()) {
    use_vertical_tabs_.SetValue(false);
  }
  if (!TabMenuModel::IsCompactNavigationModeEnabled()) {
    use_compact_navigation_bar_.SetValue(false);
  }

  UpdateTabStripModelInsertionPolicy();

  tab_restore_service_ = TabRestoreServiceFactory::GetForProfile(profile);
  if (tab_restore_service_) {
    tab_restore_service_->AddObserver(this);
    TabRestoreServiceChanged(tab_restore_service_);
  }

  if (profile_->GetProfileSyncService())
    profile_->GetProfileSyncService()->AddObserver(this);

  CreateInstantIfNecessary();

  TabFinder::GetInstance();

  UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_INIT);
}
