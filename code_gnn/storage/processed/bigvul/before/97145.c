NavigationController::~NavigationController() {
  DiscardNonCommittedEntriesInternal();

  NotificationService::current()->Notify(
      NotificationType::TAB_CLOSED,
      Source<NavigationController>(this),
      NotificationService::NoDetails());
}
