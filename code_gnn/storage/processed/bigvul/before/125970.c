NewTabObserver::NewTabObserver(AutomationProvider* automation,
                               IPC::Message* reply_message,
                               bool use_json_interface)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      use_json_interface_(use_json_interface) {
  registrar_.Add(this,
                 chrome::NOTIFICATION_TAB_PARENTED,
                 content::NotificationService::AllSources());
}
