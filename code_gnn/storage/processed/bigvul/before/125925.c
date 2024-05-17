    BrowserOpenedWithNewProfileNotificationObserver(
        AutomationProvider* automation,
        IPC::Message* reply_message)
        : automation_(automation->AsWeakPtr()),
          reply_message_(reply_message),
          new_window_id_(extension_misc::kUnknownWindowId) {
  registrar_.Add(this, chrome::NOTIFICATION_PROFILE_CREATED,
                 content::NotificationService::AllBrowserContextsAndSources());
  registrar_.Add(this, chrome::NOTIFICATION_BROWSER_OPENED,
                 content::NotificationService::AllBrowserContextsAndSources());
  registrar_.Add(this, content::NOTIFICATION_LOAD_STOP,
                 content::NotificationService::AllBrowserContextsAndSources());
}
