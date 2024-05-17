void ThreadWatcherList::StartWatching(
    const BrowserThread::ID& thread_id,
    const std::string& thread_name,
    const base::TimeDelta& sleep_time,
    const base::TimeDelta& unresponsive_time,
    uint32 unresponsive_threshold,
    const CrashOnHangThreadMap& crash_on_hang_threads) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());

  CrashOnHangThreadMap::const_iterator it =
      crash_on_hang_threads.find(thread_name);
  bool crash_on_hang = false;
  uint32 live_threads_threshold = 0;
  if (it != crash_on_hang_threads.end()) {
    crash_on_hang = true;
    live_threads_threshold = it->second.live_threads_threshold;
    unresponsive_threshold = it->second.unresponsive_threshold;
  }

  ThreadWatcher::StartWatching(
      ThreadWatcher::WatchingParams(thread_id,
                                    thread_name,
                                    sleep_time,
                                    unresponsive_time,
                                    unresponsive_threshold,
                                    crash_on_hang,
                                    live_threads_threshold));
}
