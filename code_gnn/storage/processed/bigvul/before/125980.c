void ExtensionUnloadNotificationObserver::Observe(
    int type, const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (type == chrome::NOTIFICATION_EXTENSION_UNLOADED) {
    did_receive_unload_notification_ = true;
  } else {
    NOTREACHED();
  }
}
