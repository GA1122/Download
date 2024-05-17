Browser::Browser(const CreateParams& params)
    : extension_registry_observer_(this),
      type_(params.type),
      profile_(params.profile),
      window_(NULL),
      tab_strip_model_delegate_(new chrome::BrowserTabStripModelDelegate(this)),
      tab_strip_model_(
          new TabStripModel(tab_strip_model_delegate_.get(), params.profile)),
      app_name_(params.app_name),
      is_trusted_source_(params.trusted_source),
      cancel_download_confirmation_state_(NOT_PROMPTED),
      override_bounds_(params.initial_bounds),
      initial_show_state_(params.initial_show_state),
      initial_workspace_(params.initial_workspace),
      is_session_restore_(params.is_session_restore),
      content_setting_bubble_model_delegate_(
          new BrowserContentSettingBubbleModelDelegate(this)),
      toolbar_model_delegate_(new BrowserToolbarModelDelegate(this)),
      live_tab_context_(new BrowserLiveTabContext(this)),
      synced_window_delegate_(new BrowserSyncedWindowDelegate(this)),
      bookmark_bar_state_(BookmarkBar::HIDDEN),
      command_controller_(new chrome::BrowserCommandController(this)),
      window_has_shown_(false),
      chrome_updater_factory_(this),
      weak_factory_(this) {
  CHECK(IncognitoModePrefs::CanOpenBrowser(profile_));
  CHECK(!profile_->IsGuestSession() || profile_->IsOffTheRecord())
      << "Only off the record browser may be opened in guest mode";
  DCHECK(!profile_->IsSystemProfile())
      << "The system profile should never have a real browser.";
  if (profile_->IsSystemProfile())
    content::RecordAction(base::UserMetricsAction("BrowserForSystemProfile"));

  if (IsFastTabUnloadEnabled())
    fast_unload_controller_.reset(new chrome::FastUnloadController(this));
  else
    unload_controller_.reset(new chrome::UnloadController(this));

  tab_strip_model_->AddObserver(this);

  toolbar_model_.reset(new ToolbarModelImpl(toolbar_model_delegate_.get(),
                                            content::kMaxURLDisplayChars));
  search_model_.reset(new SearchModel());
  search_delegate_.reset(new SearchDelegate(search_model_.get()));

  extension_registry_observer_.Add(
      extensions::ExtensionRegistry::Get(profile_));
  registrar_.Add(this,
                 extensions::NOTIFICATION_EXTENSION_PROCESS_TERMINATED,
                 content::NotificationService::AllSources());
#if !defined(OS_ANDROID)
  registrar_.Add(
      this, chrome::NOTIFICATION_BROWSER_THEME_CHANGED,
      content::Source<ThemeService>(
          ThemeServiceFactory::GetForProfile(profile_)));
#endif
  registrar_.Add(this, chrome::NOTIFICATION_WEB_CONTENT_SETTINGS_CHANGED,
                 content::NotificationService::AllSources());

  profile_pref_registrar_.Init(profile_->GetPrefs());
  profile_pref_registrar_.Add(
      prefs::kDevToolsDisabled,
      base::Bind(&Browser::OnDevToolsDisabledChanged, base::Unretained(this)));
  profile_pref_registrar_.Add(
      bookmarks::prefs::kShowBookmarkBar,
      base::Bind(&Browser::UpdateBookmarkBarState, base::Unretained(this),
                 BOOKMARK_BAR_STATE_CHANGE_PREF_CHANGE));

  if (search::IsInstantExtendedAPIEnabled() && is_type_tabbed())
    instant_controller_.reset(new BrowserInstantController(this));

  if (extensions::HostedAppBrowserController::IsForHostedApp(this)) {
    hosted_app_controller_.reset(
        new extensions::HostedAppBrowserController(this));
  }

  UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_INIT);

  ProfileMetrics::LogProfileLaunch(profile_);

  window_ = params.window ? params.window : CreateBrowserWindow(this);

  if (hosted_app_controller_)
    hosted_app_controller_->UpdateLocationBarVisibility(false);

  extension_window_controller_.reset(
      new BrowserExtensionWindowController(this));

  SessionService* session_service =
      SessionServiceFactory::GetForProfileForSessionRestore(profile_);
  if (session_service)
    session_service->WindowOpened(this);

  if (first_run::ShouldDoPersonalDataManagerFirstRun()) {
#if defined(OS_WIN)
    ImportAutofillDataWin(
        autofill::PersonalDataManagerFactory::GetForProfile(profile_));
#endif   
  }

  exclusive_access_manager_.reset(
      new ExclusiveAccessManager(window_->GetExclusiveAccessContext()));

  BrowserList::AddBrowser(this);
  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_BROWSER_WINDOW_READY, content::Source<Browser>(this),
      content::NotificationService::NoDetails());
}
