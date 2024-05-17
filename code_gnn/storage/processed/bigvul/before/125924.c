    BrowserOpenedWithExistingProfileNotificationObserver(
        AutomationProvider* automation,
        IPC::Message* reply_message,
        int num_loads)
        : automation_(automation->AsWeakPtr()),
          reply_message_(reply_message),
          new_window_id_(extension_misc::kUnknownWindowId),
          num_loads_(num_loads) {
  registrar_.Add(this, chrome::NOTIFICATION_BROWSER_OPENED,
                 content::NotificationService::AllBrowserContextsAndSources());
  registrar_.Add(this, content::NOTIFICATION_LOAD_STOP,
                 content::NotificationService::AllBrowserContextsAndSources());
}
