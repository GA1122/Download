void ChromeWebContentsDelegateAndroid::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_FIND_RESULT_AVAILABLE:
      OnFindResultAvailable(
          content::Source<WebContents>(source).ptr(),
          content::Details<FindNotificationDetails>(details).ptr());
      break;
    default:
      NOTREACHED() << "Unexpected notification: " << type;
      break;
  }
}
