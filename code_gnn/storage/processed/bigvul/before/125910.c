AppLaunchObserver::AppLaunchObserver(
    NavigationController* controller,
    AutomationProvider* automation,
    IPC::Message* reply_message,
    extension_misc::LaunchContainer launch_container)
    : controller_(controller),
      automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      launch_container_(launch_container),
      new_window_id_(extension_misc::kUnknownWindowId) {
  if (launch_container_ == extension_misc::LAUNCH_TAB) {
    content::Source<NavigationController> source(controller_);
    registrar_.Add(this, content::NOTIFICATION_LOAD_STOP, source);
  } else {
    registrar_.Add(this, content::NOTIFICATION_LOAD_STOP,
                   content::NotificationService::AllSources());
    registrar_.Add(this, chrome::NOTIFICATION_BROWSER_WINDOW_READY,
                   content::NotificationService::AllSources());
  }
}
