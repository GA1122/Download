Browser::Browser(const CreateParams& params)
    : extension_registry_observer_(this),
      type_(params.type),
      profile_(params.profile),
      window_(NULL),
      tab_strip_model_delegate_(new chrome::BrowserTabStripModelDelegate(this)),
      tab_strip_model_(
          std::make_unique<TabStripModel>(tab_strip_model_delegate_.get(),
                                          params.profile)),
      app_name_(params.app_name),
      is_trusted_source_(params.trusted_source),
      session_id_(SessionID::NewUnique()),
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
      hosted_app_controller_(MaybeCreateHostedAppController(this)),
      bookmark_bar_state_(BookmarkBar::HIDDEN),
      command_controller_(new chrome::BrowserCommandController(this)),
      window_has_shown_(false),
      chrome_updater_factory_(this),
      weak_factory_(this) {
  CHECK(IncognitoModePrefs::CanOpenBrowser(profile_));
  CHECK(!profile_->IsGuestSession() || profile_->IsOffTheRecord())
      << "Only off the record browser may be opened in guest mode";
  CHECK(!profile_->IsSystemProfile())
      << "The system profile should never have a real browser.";

  if (IsFastTabUnloadEnabled())
    fast_unload_controller_.reset(new FastUnloadController(this));
  else
    unload_controller_.reset(new UnloadController(this));

  tab_strip_model_->AddObserver(this);

  toolbar_model_.reset(new ToolbarModelImpl(toolbar_model_delegate_.get(),
                                            content::kMaxURLDisplayChars));

  extension_registry_observer_.Add(
      extensions::ExtensionRegistry::Get(profile_));
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
      prefs::kDevToolsAvailability,
      base::BindRepeating(&Browser::OnDevToolsAvailabilityChanged,
                          base::Unretained(this)));
  profile_pref_registrar_.Add(
      bookmarks::prefs::kShowBookmarkBar,
      base::BindRepeating(&Browser::UpdateBookmarkBarState,
                          base::Unretained(this),
                          BOOKMARK_BAR_STATE_CHANGE_PREF_CHANGE));

  if (search::IsInstantExtendedAPIEnabled() && is_type_tabbed())
    instant_controller_.reset(new BrowserInstantController(this));

  UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_INIT);

  ProfileMetrics::LogProfileLaunch(profile_);

  if (params.skip_window_init_for_testing)
    return;

  window_ = params.window ? params.window
                          : CreateBrowserWindow(this, params.user_gesture);

  if (hosted_app_controller_)
    hosted_app_controller_->UpdateLocationBarVisibility(false);

  extension_window_controller_.reset(
      new extensions::BrowserExtensionWindowController(this));

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
}
