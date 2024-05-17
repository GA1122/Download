void NotifyPrunedEntries(NavigationController* nav_controller,
                         bool from_front,
                         int count) {
  content::PrunedDetails details;
  details.from_front = from_front;
  details.count = count;
  content::NotificationService::current()->Notify(
      content::NOTIFICATION_NAV_LIST_PRUNED,
      content::Source<NavigationController>(nav_controller),
      content::Details<content::PrunedDetails>(&details));
}
