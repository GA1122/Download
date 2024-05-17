void ThreadWatcherList::GetStatusOfThreads(uint32* responding_thread_count,
                                           uint32* unresponding_thread_count) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  *responding_thread_count = 0;
  *unresponding_thread_count = 0;
  if (!g_thread_watcher_list_)
    return;

  for (RegistrationList::iterator it =
           g_thread_watcher_list_->registered_.begin();
       g_thread_watcher_list_->registered_.end() != it;
       ++it) {
    if (it->second->IsVeryUnresponsive())
      ++(*unresponding_thread_count);
    else
      ++(*responding_thread_count);
  }
}
