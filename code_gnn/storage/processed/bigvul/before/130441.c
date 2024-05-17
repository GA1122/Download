void ThreadWatcherList::WakeUpAll() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  if (!g_thread_watcher_list_)
    return;

  for (RegistrationList::iterator it =
           g_thread_watcher_list_->registered_.begin();
       g_thread_watcher_list_->registered_.end() != it;
       ++it)
    it->second->WakeUp();
}
