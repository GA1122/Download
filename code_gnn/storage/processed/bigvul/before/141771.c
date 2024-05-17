    AreNotificationListenersEnabledOnAllProfiles() {
  if (g_notification_listeners_failed)
    return false;
  return notification_listeners_active_;
}
