void OmniboxAcceptNotificationObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (type == content::NOTIFICATION_LOAD_STOP ||
      type == chrome::NOTIFICATION_AUTH_NEEDED) {
    if (automation_) {
      AutomationJSONReply(automation_,
                          reply_message_.release()).SendSuccess(NULL);
    }
    delete this;
  } else {
    NOTREACHED();
  }
}
