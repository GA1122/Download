void ExtensionService::InitEventRouters() {
  if (event_routers_initialized_)
    return;

  ExtensionHistoryEventRouter::GetInstance()->ObserveProfile(profile_);
  ExtensionAccessibilityEventRouter::GetInstance()->ObserveProfile(profile_);
  browser_event_router_.reset(new ExtensionBrowserEventRouter(profile_));
  browser_event_router_->Init();
  preference_event_router_.reset(new ExtensionPreferenceEventRouter(profile_));
  ExtensionBookmarkEventRouter::GetInstance()->Observe(
      profile_->GetBookmarkModel());
  ExtensionCookiesEventRouter::GetInstance()->Init();
  ExtensionManagementEventRouter::GetInstance()->Init();
  ExtensionProcessesEventRouter::GetInstance()->ObserveProfile(profile_);
  ExtensionWebNavigationEventRouter::GetInstance()->Init();

#if defined(OS_CHROMEOS)
  ExtensionFileBrowserEventRouter::GetInstance()->ObserveFileSystemEvents(
      profile_);
  ExtensionMediaPlayerEventRouter::GetInstance()->Init(profile_);
#endif

#if defined(OS_CHROMEOS) && defined(TOUCH_UI)
  ExtensionInputUiEventRouter::GetInstance()->Init();
#endif

  event_routers_initialized_ = true;
}
