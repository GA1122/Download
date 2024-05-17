BrowserClosedNotificationObserver::BrowserClosedNotificationObserver(
    Browser* browser,
    AutomationProvider* automation,
    IPC::Message* reply_message,
    bool use_json_interface)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      use_json_interface_(use_json_interface),
      for_browser_command_(false) {
  registrar_.Add(this, chrome::NOTIFICATION_BROWSER_CLOSED,
                 content::Source<Browser>(browser));
}
