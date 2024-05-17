ExecuteBrowserCommandObserver::ExecuteBrowserCommandObserver(
    AutomationProvider* automation,
    IPC::Message* reply_message,
    bool use_json_interface)
    : automation_(automation->AsWeakPtr()),
      notification_type_(content::NOTIFICATION_ALL),
      reply_message_(reply_message),
      use_json_interface_(use_json_interface) {
}
