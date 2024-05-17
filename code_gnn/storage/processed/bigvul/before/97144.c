void NavigationController::SetWindowID(const SessionID& id) {
  window_id_ = id;
  NotificationService::current()->Notify(NotificationType::TAB_PARENTED,
                                         Source<NavigationController>(this),
                                         NotificationService::NoDetails());
}
