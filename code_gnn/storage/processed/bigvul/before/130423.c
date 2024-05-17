void ThreadWatcherList::Register(ThreadWatcher* watcher) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  if (!g_thread_watcher_list_)
    return;
  DCHECK(!g_thread_watcher_list_->Find(watcher->thread_id()));
  g_thread_watcher_list_->registered_[watcher->thread_id()] = watcher;
}
