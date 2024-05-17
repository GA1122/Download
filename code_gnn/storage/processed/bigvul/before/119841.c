void RegisterForAllNavNotifications(TestNotificationTracker* tracker,
                                    NavigationController* controller) {
  tracker->ListenFor(NOTIFICATION_NAV_LIST_PRUNED,
                     Source<NavigationController>(controller));
  tracker->ListenFor(NOTIFICATION_NAV_ENTRY_CHANGED,
                     Source<NavigationController>(controller));
}
