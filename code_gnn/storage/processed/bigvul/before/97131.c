void NotifyPrunedEntries(NavigationController* nav_controller,
                         bool from_front,
                         int count) {
  NavigationController::PrunedDetails details;
  details.from_front = from_front;
  details.count = count;
  NotificationService::current()->Notify(
      NotificationType::NAV_LIST_PRUNED,
      Source<NavigationController>(nav_controller),
      Details<NavigationController::PrunedDetails>(&details));
}
