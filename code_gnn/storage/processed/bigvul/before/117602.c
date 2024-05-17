void RegisterLocalState(PrefService* local_state) {
  local_state->RegisterIntegerPref(prefs::kMultipleProfilePrefMigration, 0);
  AppsPromo::RegisterPrefs(local_state);
  Browser::RegisterPrefs(local_state);
  FlagsUI::RegisterPrefs(local_state);
  WebCacheManager::RegisterPrefs(local_state);
  ExternalProtocolHandler::RegisterPrefs(local_state);
  GoogleURLTracker::RegisterPrefs(local_state);
  IntranetRedirectDetector::RegisterPrefs(local_state);
  KeywordEditorController::RegisterPrefs(local_state);
  MetricsLog::RegisterPrefs(local_state);
  MetricsService::RegisterPrefs(local_state);
  NewTabPageHandler::RegisterPrefs(local_state);
  printing::PrintJobManager::RegisterPrefs(local_state);
  PromoResourceService::RegisterPrefs(local_state);

#if defined(ENABLE_SAFE_BROWSING)
  SafeBrowsingService::RegisterPrefs(local_state);
#endif
  browser_shutdown::RegisterPrefs(local_state);
#if defined(TOOLKIT_VIEWS)
  BrowserView::RegisterBrowserViewPrefs(local_state);
#endif
  UpgradeDetector::RegisterPrefs(local_state);
  TaskManager::RegisterPrefs(local_state);
  geolocation::RegisterPrefs(local_state);
  BackgroundModeManager::RegisterPrefs(local_state);
  NotificationUIManager::RegisterPrefs(local_state);
  PrefProxyConfigTrackerImpl::RegisterPrefs(local_state);
  SSLConfigServiceManager::RegisterPrefs(local_state);
#if defined(ENABLE_CONFIGURATION_POLICY)
  policy::CloudPolicySubsystem::RegisterPrefs(local_state);
#endif
  ProfileInfoCache::RegisterPrefs(local_state);
  ProfileManager::RegisterPrefs(local_state);
#if defined(OS_CHROMEOS)
  chromeos::AudioMixerAlsa::RegisterPrefs(local_state);
  chromeos::UserManager::RegisterPrefs(local_state);
  chromeos::BaseLoginDisplayHost::RegisterPrefs(local_state);
  chromeos::WizardController::RegisterPrefs(local_state);
  chromeos::InputMethodMenu::RegisterPrefs(local_state);
  chromeos::ServicesCustomizationDocument::RegisterPrefs(local_state);
  chromeos::signed_settings_cache::RegisterPrefs(local_state);
  chromeos::NetworkMenuButton::RegisterPrefs(local_state);
  chromeos::ProxyConfigServiceImpl::RegisterPrefs(local_state);
  policy::DeviceStatusCollector::RegisterPrefs(local_state);
#endif
}
