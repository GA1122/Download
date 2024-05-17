void ExtensionDevToolsClientHost::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_APP_TERMINATING:
      Close();
      break;
    case chrome::NOTIFICATION_TAB_CONTENTS_INFOBAR_REMOVED:
      if (content::Details<infobars::InfoBar::RemovedDetails>(details)->first ==
          infobar_) {
        infobar_ = NULL;
        SendDetachedEvent();
        Close();
      }
      break;
    default:
      NOTREACHED();
  }
}
