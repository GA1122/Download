ThreadWatcherList::~ThreadWatcherList() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  DCHECK(this == g_thread_watcher_list_);
  g_thread_watcher_list_ = nullptr;
}
