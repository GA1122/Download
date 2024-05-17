void NavigationControllerRestoredObserver::Observe(
    int type, const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (FinishedRestoring()) {
    registrar_.RemoveAll();
    SendDone();
  }
}
