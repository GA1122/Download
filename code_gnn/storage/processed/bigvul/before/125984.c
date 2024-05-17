void BrowserCountChangeNotificationObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK(type == chrome::NOTIFICATION_BROWSER_OPENED ||
         type == chrome::NOTIFICATION_BROWSER_CLOSED);
  int current_count = static_cast<int>(BrowserList::size());
  if (type == chrome::NOTIFICATION_BROWSER_CLOSED) {
    DCHECK_LT(0, current_count);
    current_count--;
  }

  if (!automation_) {
    delete this;
    return;
  }

  if (current_count == target_count_) {
    AutomationMsg_WaitForBrowserWindowCountToBecome::WriteReplyParams(
        reply_message_.get(), true);
    automation_->Send(reply_message_.release());
    delete this;
  }
}
