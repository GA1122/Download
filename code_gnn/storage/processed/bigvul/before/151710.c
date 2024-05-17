void Browser::OnWindowClosing() {
  if (!ShouldCloseWindow())
    return;

  bool should_quit_if_last_browser =
      browser_shutdown::IsTryingToQuit() ||
      !KeepAliveRegistry::GetInstance()->IsKeepingAlive();

  if (should_quit_if_last_browser && ShouldStartShutdown())
    browser_shutdown::OnShutdownStarting(browser_shutdown::WINDOW_CLOSE);

  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile());
  if (session_service)
    session_service->WindowClosing(session_id());

  sessions::TabRestoreService* tab_restore_service =
      TabRestoreServiceFactory::GetForProfile(profile());

#if defined(USE_AURA)
  if (tab_restore_service && is_app() && !is_devtools())
    tab_restore_service->BrowserClosing(live_tab_context());
#endif

  if (tab_restore_service && is_type_tabbed() && tab_strip_model_->count())
    tab_restore_service->BrowserClosing(live_tab_context());

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_BROWSER_CLOSING,
      content::Source<Browser>(this),
      content::NotificationService::NoDetails());

  if (!IsFastTabUnloadEnabled())
    tab_strip_model_->CloseAllTabs();
}
