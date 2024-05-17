WindowMaximizedObserver::WindowMaximizedObserver(
    AutomationProvider* automation,
    IPC::Message* reply_message)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message) {
  registrar_.Add(this, chrome::NOTIFICATION_BROWSER_WINDOW_MAXIMIZED,
                 content::NotificationService::AllSources());
}
