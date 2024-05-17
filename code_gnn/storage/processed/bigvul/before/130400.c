ThreadWatcher* ThreadWatcherList::Find(const BrowserThread::ID& thread_id) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  if (!g_thread_watcher_list_)
    return nullptr;
  RegistrationList::iterator it =
      g_thread_watcher_list_->registered_.find(thread_id);
  if (g_thread_watcher_list_->registered_.end() == it)
    return nullptr;
  return it->second;
}
