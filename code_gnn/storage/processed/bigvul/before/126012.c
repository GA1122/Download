OmniboxAcceptNotificationObserver::OmniboxAcceptNotificationObserver(
    NavigationController* controller,
    AutomationProvider* automation,
    IPC::Message* reply_message)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      controller_(controller) {
  content::Source<NavigationController> source(controller_);
  registrar_.Add(this, content::NOTIFICATION_LOAD_STOP, source);
  registrar_.Add(this, chrome::NOTIFICATION_AUTH_NEEDED, source);
}
