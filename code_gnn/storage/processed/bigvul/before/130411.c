void ThreadWatcherObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  OnUserActivityDetected();
}
