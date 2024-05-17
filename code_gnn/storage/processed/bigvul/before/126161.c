Browser::Browser(const CreateParams& params)
    : type_(params.type),
      profile_(params.profile),
      window_(NULL),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          tab_strip_model_delegate_(
            new chrome::BrowserTabStripModelDelegate(this))),
      tab_strip_model_(new TabStripModel(tab_strip_model_delegate_.get(),
                                         params.profile)),
      app_name_(params.app_name),
      app_type_(params.app_type),
      chrome_updater_factory_(this),
      cancel_download_confirmation_state_(NOT_PROMPTED),
      override_bounds_(params.initial_bounds),
      initial_show_state_(params.initial_show_state),
      is_session_restore_(params.is_session_restore),
      host_desktop_type_(params.host_desktop_type),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          unload_controller_(new chrome::UnloadController(this))),
      weak_factory_(this),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          content_setting_bubble_model_delegate_(
              new BrowserContentSettingBubbleModelDelegate(this))),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          toolbar_model_delegate_(
              new BrowserToolbarModelDelegate(this))),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          tab_restore_service_delegate_(
              new BrowserTabRestoreServiceDelegate(this))),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          synced_window_delegate_(
              new BrowserSyncedWindowDelegate(this))),
      bookmark_bar_state_(BookmarkBar::HIDDEN),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          command_controller_(new chrome::BrowserCommandController(this))),
      window_has_shown_(false) {
  if (!app_name_.empty())
    chrome::RegisterAppPrefs(app_name_, profile_);
  tab_strip_model_->AddObserver(this);

  toolbar_model_.reset(new ToolbarModelImpl(toolbar_model_delegate_.get()));
  search_model_.reset(new chrome::search::SearchModel(NULL));
  search_delegate_.reset(
      new chrome::search::SearchDelegate(search_model_.get(),
                                         toolbar_model_.get()));

  registrar_.Add(this, content::NOTIFICATION_SSL_VISIBLE_STATE_CHANGED,
                 content::NotificationService::AllSources());
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_LOADED,
                 content::Source<Profile>(profile_->GetOriginalProfile()));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNLOADED,
                 content::Source<Profile>(profile_->GetOriginalProfile()));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNINSTALLED,
                 content::Source<Profile>(profile_->GetOriginalProfile()));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_PROCESS_TERMINATED,
                 content::NotificationService::AllSources());
#if defined(ENABLE_THEMES)
  registrar_.Add(
      this, chrome::NOTIFICATION_BROWSER_THEME_CHANGED,
      content::Source<ThemeService>(
          ThemeServiceFactory::GetForProfile(profile_)));
#endif
  registrar_.Add(this, chrome::NOTIFICATION_WEB_CONTENT_SETTINGS_CHANGED,
                 content::NotificationService::AllSources());

  profile_pref_registrar_.Init(profile_->GetPrefs());
  profile_pref_registrar_.Add(prefs::kDevToolsDisabled, this);
  profile_pref_registrar_.Add(prefs::kShowBookmarkBar, this);
  profile_pref_registrar_.Add(prefs::kHomePage, this);

  BrowserList::AddBrowser(this);

  encoding_auto_detect_.Init(prefs::kWebKitUsesUniversalDetector,
                             profile_->GetPrefs(), NULL);

  instant_controller_.reset(new chrome::BrowserInstantController(this));

#if 0
  device_attached_intent_source_.reset(
      new DeviceAttachedIntentSource(this, (this)));
#endif

  UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_INIT);

  FilePath profile_path = profile_->GetPath();
  ProfileMetrics::LogProfileLaunch(profile_path);

  window_ = params.window ? params.window : CreateBrowserWindow(this);

#if defined(OS_WIN) && !defined(USE_AURA)
  ui::win::SetAppIdForWindow(
      is_app() && !is_type_panel() ?
      ShellIntegration::GetAppModelIdForProfile(UTF8ToWide(app_name_),
                                                profile_->GetPath()) :
      ShellIntegration::GetChromiumModelIdForProfile(profile_->GetPath()),
      window()->GetNativeWindow());

  if (is_type_panel()) {
    ui::win::SetAppIconForWindow(ShellIntegration::GetChromiumIconPath(),
                                 window()->GetNativeWindow());
  }
#endif

  extension_window_controller_.reset(
      new BrowserExtensionWindowController(this));

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_BROWSER_WINDOW_READY,
      content::Source<Browser>(this),
      content::NotificationService::NoDetails());

  PrefService* local_state = g_browser_process->local_state();
  if (local_state && local_state->FindPreference(
      prefs::kAutofillPersonalDataManagerFirstRun) &&
      local_state->GetBoolean(prefs::kAutofillPersonalDataManagerFirstRun)) {
#if defined(OS_WIN)
    ImportAutofillDataWin(PersonalDataManagerFactory::GetForProfile(profile_));
#endif   
    local_state->ClearPref(prefs::kAutofillPersonalDataManagerFirstRun);
  }

  fullscreen_controller_.reset(new FullscreenController(this));
  search_model_->AddObserver(this);
}
