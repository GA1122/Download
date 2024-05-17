void ThreadWatcherList::SetStopped(bool stopped) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  g_stopped_ = stopped;
}
