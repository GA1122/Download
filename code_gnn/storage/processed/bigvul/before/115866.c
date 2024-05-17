void RegisterForAllNavNotifications(TestNotificationTracker* tracker,
                                    NavigationController* controller) {
  tracker->ListenFor(content::NOTIFICATION_NAV_ENTRY_COMMITTED,
                     content::Source<NavigationController>(controller));
  tracker->ListenFor(content::NOTIFICATION_NAV_LIST_PRUNED,
                     content::Source<NavigationController>(controller));
  tracker->ListenFor(content::NOTIFICATION_NAV_ENTRY_CHANGED,
                     content::Source<NavigationController>(controller));
}
