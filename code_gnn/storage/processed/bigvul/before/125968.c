NavigationNotificationObserver::NavigationNotificationObserver(
    NavigationController* controller,
    AutomationProvider* automation,
    IPC::Message* reply_message,
    int number_of_navigations,
    bool include_current_navigation,
    bool use_json_interface)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      controller_(controller),
      navigations_remaining_(number_of_navigations),
      navigation_started_(false),
      use_json_interface_(use_json_interface) {
  if (number_of_navigations == 0) {
    ConditionMet(AUTOMATION_MSG_NAVIGATION_SUCCESS);
    return;
  }
  DCHECK_LT(0, navigations_remaining_);
  content::Source<NavigationController> source(controller_);
  registrar_.Add(this, content::NOTIFICATION_NAV_ENTRY_COMMITTED, source);
  registrar_.Add(this, content::NOTIFICATION_LOAD_START, source);
  registrar_.Add(this, content::NOTIFICATION_LOAD_STOP, source);
  registrar_.Add(this, chrome::NOTIFICATION_AUTH_NEEDED, source);
  registrar_.Add(this, chrome::NOTIFICATION_AUTH_SUPPLIED, source);
  registrar_.Add(this, chrome::NOTIFICATION_AUTH_CANCELLED, source);
  registrar_.Add(this, chrome::NOTIFICATION_APP_MODAL_DIALOG_SHOWN,
                 content::NotificationService::AllSources());

  if (include_current_navigation && controller->GetWebContents()->IsLoading())
    navigation_started_ = true;
}
