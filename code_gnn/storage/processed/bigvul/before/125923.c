BrowserOpenedNotificationObserver::BrowserOpenedNotificationObserver(
    AutomationProvider* automation,
    IPC::Message* reply_message,
    bool use_json_interface)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      new_window_id_(extension_misc::kUnknownWindowId),
      use_json_interface_(use_json_interface),
      for_browser_command_(false) {
  registrar_.Add(this, chrome::NOTIFICATION_BROWSER_OPENED,
                 content::NotificationService::AllBrowserContextsAndSources());
  registrar_.Add(this, content::NOTIFICATION_LOAD_STOP,
                 content::NotificationService::AllBrowserContextsAndSources());
}
