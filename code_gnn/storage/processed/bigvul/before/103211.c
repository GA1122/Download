bool Browser::TakeFocus(bool reverse) {
  NotificationService::current()->Notify(
      NotificationType::FOCUS_RETURNED_TO_BROWSER,
      Source<Browser>(this),
      NotificationService::NoDetails());
  return false;
}
