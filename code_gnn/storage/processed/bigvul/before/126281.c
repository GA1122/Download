void BrowserCommandController::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  switch (type) {
    case content::NOTIFICATION_INTERSTITIAL_ATTACHED:
      UpdateCommandsForTabState();
      break;

    case content::NOTIFICATION_INTERSTITIAL_DETACHED:
      UpdateCommandsForTabState();
      break;

    default:
      NOTREACHED() << "Got a notification we didn't register for.";
  }
}
