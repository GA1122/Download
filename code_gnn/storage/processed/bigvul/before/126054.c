TabClosedNotificationObserver::TabClosedNotificationObserver(
    AutomationProvider* automation,
    bool wait_until_closed,
    IPC::Message* reply_message,
    bool use_json_interface)
    : TabStripNotificationObserver((wait_until_closed ?
          static_cast<int>(content::NOTIFICATION_WEB_CONTENTS_DESTROYED) :
          static_cast<int>(chrome::NOTIFICATION_TAB_CLOSING)), automation),
      reply_message_(reply_message),
      use_json_interface_(use_json_interface),
      for_browser_command_(false) {
}
