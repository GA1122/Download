ThreadWatcherObserver::~ThreadWatcherObserver() {
  DCHECK(this == g_thread_watcher_observer_);
  g_thread_watcher_observer_ = nullptr;
}
