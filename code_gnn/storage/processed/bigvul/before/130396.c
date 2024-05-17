void ThreadWatcherList::DeleteAll() {
  if (!WatchDogThread::CurrentlyOnWatchDogThread()) {
    WatchDogThread::PostTask(
        FROM_HERE,
        base::Bind(&ThreadWatcherList::DeleteAll));
    return;
  }

  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());

  SetStopped(true);

  if (!g_thread_watcher_list_)
    return;

  while (!g_thread_watcher_list_->registered_.empty()) {
    RegistrationList::iterator it = g_thread_watcher_list_->registered_.begin();
    delete it->second;
    g_thread_watcher_list_->registered_.erase(it);
  }

  delete g_thread_watcher_list_;
}
