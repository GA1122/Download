void Browser::OnWindowClosing() {
  if (!ShouldCloseWindow())
    return;

  bool should_quit_if_last_browser =
      browser_shutdown::IsTryingToQuit() || !browser::WillKeepAlive();

  if (should_quit_if_last_browser && BrowserList::size() == 1)
    browser_shutdown::OnShutdownStarting(browser_shutdown::WINDOW_CLOSE);

  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile());
  if (session_service)
    session_service->WindowClosing(session_id());

  TabRestoreService* tab_restore_service =
      TabRestoreServiceFactory::GetForProfile(profile());

#if defined(USE_AURA)
  if (tab_restore_service && is_app())
    tab_restore_service->BrowserClosing(tab_restore_service_delegate());
#endif

  if (tab_restore_service && is_type_tabbed() && tab_count())
    tab_restore_service->BrowserClosing(tab_restore_service_delegate());

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_BROWSER_CLOSING,
      content::Source<Browser>(this),
      content::NotificationService::NoDetails());

  chrome::CloseAllTabs(this);
}
