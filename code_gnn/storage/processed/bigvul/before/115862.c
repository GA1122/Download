NavigationController::~NavigationController() {
  DiscardNonCommittedEntriesInternal();

  content::NotificationService::current()->Notify(
      content::NOTIFICATION_TAB_CLOSED,
      content::Source<NavigationController>(this),
      content::NotificationService::NoDetails());
}
