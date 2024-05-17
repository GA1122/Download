void ThreadWatcherList::InitializeAndStartWatching(
    uint32 unresponsive_threshold,
    const CrashOnHangThreadMap& crash_on_hang_threads) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());

  BrowserThread::PostTask(
      BrowserThread::UI,
      FROM_HERE,
      base::Bind(&StartupTimeBomb::DisarmStartupTimeBomb));

  if (g_thread_watcher_list_ || g_stopped_)
    return;

  ThreadWatcherList* thread_watcher_list = new ThreadWatcherList();
  CHECK(thread_watcher_list);

  version_info::Channel channel = chrome::GetChannel();
  if (channel == version_info::Channel::STABLE ||
      channel == version_info::Channel::UNKNOWN) {
    return;
  }

  const base::TimeDelta kSleepTime =
      base::TimeDelta::FromSeconds(kSleepSeconds);
  const base::TimeDelta kUnresponsiveTime =
      base::TimeDelta::FromSeconds(kUnresponsiveSeconds);

  StartWatching(BrowserThread::UI, "UI", kSleepTime, kUnresponsiveTime,
                unresponsive_threshold, crash_on_hang_threads);
  StartWatching(BrowserThread::IO, "IO", kSleepTime, kUnresponsiveTime,
                unresponsive_threshold, crash_on_hang_threads);
  StartWatching(BrowserThread::DB, "DB", kSleepTime, kUnresponsiveTime,
                unresponsive_threshold, crash_on_hang_threads);
  StartWatching(BrowserThread::FILE, "FILE", kSleepTime, kUnresponsiveTime,
                unresponsive_threshold, crash_on_hang_threads);
  StartWatching(BrowserThread::CACHE, "CACHE", kSleepTime, kUnresponsiveTime,
                unresponsive_threshold, crash_on_hang_threads);
}
