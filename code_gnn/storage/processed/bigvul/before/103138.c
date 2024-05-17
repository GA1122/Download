void Browser::OnWindowClosing() {
  if (!ShouldCloseWindow())
    return;

  bool exiting = false;

  bool should_quit_if_last_browser =
      browser_shutdown::IsTryingToQuit() || !BrowserList::WillKeepAlive();

  if (should_quit_if_last_browser && BrowserList::size() == 1) {
    browser_shutdown::OnShutdownStarting(browser_shutdown::WINDOW_CLOSE);
    exiting = true;
  }

  SessionService* session_service = profile()->GetSessionService();
  if (session_service)
    session_service->WindowClosing(session_id());

  TabRestoreService* tab_restore_service = profile()->GetTabRestoreService();
  if (tab_restore_service && type() == TYPE_NORMAL && tab_count())
    tab_restore_service->BrowserClosing(tab_restore_service_delegate());

  NotificationService::current()->Notify(
      NotificationType::BROWSER_CLOSING,
      Source<Browser>(this),
      Details<bool>(&exiting));

  CloseAllTabs();
}
