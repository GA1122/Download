void NewTabUILoadObserver::Observe(int type,
                                   const content::NotificationSource& source,
                                   const content::NotificationDetails& details) {
  if (type == chrome::NOTIFICATION_INITIAL_NEW_TAB_UI_LOAD) {
    content::Details<int> load_time(details);
    if (automation_) {
      automation_->Send(
          new AutomationMsg_InitialNewTabUILoadComplete(*load_time.ptr()));
    }
  } else {
    NOTREACHED();
  }
}
