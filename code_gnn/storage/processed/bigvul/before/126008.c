void WindowMaximizedObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK_EQ(chrome::NOTIFICATION_BROWSER_WINDOW_MAXIMIZED, type);

  if (automation_) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendSuccess(NULL);
  }
  delete this;
}
