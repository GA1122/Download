void SavePackageNotificationObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (type == content::NOTIFICATION_SAVE_PACKAGE_SUCCESSFULLY_FINISHED) {
    if (automation_) {
      AutomationJSONReply(automation_,
                          reply_message_.release()).SendSuccess(NULL);
    }
    delete this;
  } else {
    NOTREACHED();
  }
}
