void BrowserOpenedWithExistingProfileNotificationObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (!automation_) {
    delete this;
    return;
  }

  if (type == chrome::NOTIFICATION_BROWSER_OPENED) {
    new_window_id_ = ExtensionTabUtil::GetWindowId(
        content::Source<Browser>(source).ptr());
  } else if (type == content::NOTIFICATION_LOAD_STOP) {
    NavigationController* controller =
        content::Source<NavigationController>(source).ptr();
    SessionTabHelper* session_tab_helper =
        SessionTabHelper::FromWebContents(controller->GetWebContents());
    int window_id = session_tab_helper ? session_tab_helper->window_id().id()
                                       : -1;
    if (window_id == new_window_id_ && --num_loads_ == 0) {
      if (automation_) {
        AutomationJSONReply(automation_, reply_message_.release())
            .SendSuccess(NULL);
      }
      delete this;
    }
  } else {
    NOTREACHED();
  }
}
