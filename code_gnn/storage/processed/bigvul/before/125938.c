ExtensionReadyNotificationObserver::ExtensionReadyNotificationObserver(
    ExtensionProcessManager* manager, ExtensionService* service,
    AutomationProvider* automation, IPC::Message* reply_message)
    : manager_(manager),
      service_(service),
      automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      extension_(NULL) {
  Init();
}
