void ThreadWatcherList::StopWatchingAll() {
  ThreadWatcherObserver::RemoveNotifications();
  DeleteAll();
}
