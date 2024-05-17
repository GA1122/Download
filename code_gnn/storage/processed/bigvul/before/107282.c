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
      maximized_state_(MAXIMIZED_STATE_DEFAULT),
      method_factory_(this),
      block_command_execution_(false),
      last_blocked_command_id_(-1),
      last_blocked_command_disposition_(CURRENT_TAB),
      pending_web_app_action_(NONE) {
  registrar_.Add(this, NotificationType::SSL_VISIBLE_STATE_CHANGED,
                 NotificationService::AllSources());
  registrar_.Add(this, NotificationType::EXTENSION_UPDATE_DISABLED,
                 NotificationService::AllSources());
  registrar_.Add(this, NotificationType::EXTENSION_LOADED,
                 NotificationService::AllSources());
  registrar_.Add(this, NotificationType::EXTENSION_UNLOADED,
                 NotificationService::AllSources());
  registrar_.Add(this, NotificationType::EXTENSION_PROCESS_TERMINATED,
                 NotificationService::AllSources());
  registrar_.Add(this, NotificationType::BROWSER_THEME_CHANGED,
                 NotificationService::AllSources());
  registrar_.Add(this, NotificationType::PROFILE_ERROR,
                 NotificationService::AllSources());

  registrar_.Add(this, NotificationType::EXTENSION_READY_FOR_INSTALL,
                 NotificationService::AllSources());

  PrefService* local_state = g_browser_process->local_state();
  if (local_state)
    printing_enabled_.Init(prefs::kPrintingEnabled, local_state, this);
  dev_tools_disabled_.Init(prefs::kDevToolsDisabled,
                           profile_->GetPrefs(), this);
  incognito_mode_allowed_.Init(prefs::kIncognitoEnabled,
                               profile_->GetPrefs(), this);

  InitCommandState();
  BrowserList::AddBrowser(this);

  encoding_auto_detect_.Init(prefs::kWebKitUsesUniversalDetector,
                             profile_->GetPrefs(), NULL);
  use_vertical_tabs_.Init(prefs::kUseVerticalTabs, profile_->GetPrefs(), this);
  instant_enabled_.Init(prefs::kInstantEnabled, profile_->GetPrefs(), this);
  if (!TabMenuModel::AreVerticalTabsEnabled()) {
    use_vertical_tabs_.SetValue(false);
  }
  UpdateTabStripModelInsertionPolicy();

  tab_restore_service_ = profile->GetTabRestoreService();
  if (tab_restore_service_) {
    tab_restore_service_->AddObserver(this);
    TabRestoreServiceChanged(tab_restore_service_);
  }

  if (profile_->GetProfileSyncService())
    profile_->GetProfileSyncService()->AddObserver(this);

  CreateInstantIfNecessary();
}
