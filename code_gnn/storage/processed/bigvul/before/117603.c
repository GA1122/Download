void RegisterUserPrefs(PrefService* user_prefs) {
  AppsPromo::RegisterUserPrefs(user_prefs);
  AutofillManager::RegisterUserPrefs(user_prefs);
  SessionStartupPref::RegisterUserPrefs(user_prefs);
  BookmarkModel::RegisterUserPrefs(user_prefs);
  Browser::RegisterUserPrefs(user_prefs);
  BrowserInit::RegisterUserPrefs(user_prefs);
  PasswordManager::RegisterUserPrefs(user_prefs);
  chrome_browser_net::Predictor::RegisterUserPrefs(user_prefs);
  DownloadPrefs::RegisterUserPrefs(user_prefs);
  bookmark_utils::RegisterUserPrefs(user_prefs);
  AlternateErrorPageTabObserver::RegisterUserPrefs(user_prefs);
  PrefsTabHelper::RegisterUserPrefs(user_prefs);
  TemplateURLPrepopulateData::RegisterUserPrefs(user_prefs);
  ExtensionWebUI::RegisterUserPrefs(user_prefs);
  ExtensionSettingsHandler::RegisterUserPrefs(user_prefs);
  IncognitoModePrefs::RegisterUserPrefs(user_prefs);
  NewTabUI::RegisterUserPrefs(user_prefs);
  PluginsUI::RegisterUserPrefs(user_prefs);
  ProfileImpl::RegisterUserPrefs(user_prefs);
  PromoResourceService::RegisterUserPrefs(user_prefs);
  HostContentSettingsMap::RegisterUserPrefs(user_prefs);
  CookieSettings::RegisterUserPrefs(user_prefs);
  DevToolsWindow::RegisterUserPrefs(user_prefs);
  PinnedTabCodec::RegisterUserPrefs(user_prefs);
  ExtensionPrefs::RegisterUserPrefs(user_prefs);
  TranslatePrefs::RegisterUserPrefs(user_prefs);
  PrefProxyConfigTrackerImpl::RegisterPrefs(user_prefs);
#if defined(TOOLKIT_VIEWS)
  BrowserActionsContainer::RegisterUserPrefs(user_prefs);
#elif defined(TOOLKIT_GTK)
  BrowserWindowGtk::RegisterUserPrefs(user_prefs);
#endif
#if defined(OS_CHROMEOS)
  chromeos::Preferences::RegisterUserPrefs(user_prefs);
  chromeos::ProxyConfigServiceImpl::RegisterPrefs(user_prefs);
#endif
  SigninManager::RegisterUserPrefs(user_prefs);
  InstantController::RegisterUserPrefs(user_prefs);
  NetPrefObserver::RegisterPrefs(user_prefs);
  ProtocolHandlerRegistry::RegisterPrefs(user_prefs);
#if defined(OS_MACOSX)
  PresentationModePrefs::RegisterUserPrefs(user_prefs);
#endif
#if defined(ENABLE_CONFIGURATION_POLICY)
  policy::URLBlacklistManager::RegisterPrefs(user_prefs);
#endif
  SyncPromoUI::RegisterUserPrefs(user_prefs);
  GAIAInfoUpdateService::RegisterUserPrefs(user_prefs);
  chrome_browser_net::HttpServerPropertiesManager::RegisterPrefs(user_prefs);
#if !defined(OS_CHROMEOS)
  default_apps::RegisterUserPrefs(user_prefs);
#endif
  extensions::ComponentLoader::RegisterUserPrefs(user_prefs);
}
