void ThreadWatcherList::StartWatchingAll(
    const base::CommandLine& command_line) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  uint32 unresponsive_threshold;
  CrashOnHangThreadMap crash_on_hang_threads;
  ParseCommandLine(command_line,
                   &unresponsive_threshold,
                   &crash_on_hang_threads);

  ThreadWatcherObserver::SetupNotifications(
      base::TimeDelta::FromSeconds(kSleepSeconds * unresponsive_threshold));

  WatchDogThread::PostTask(
      FROM_HERE,
      base::Bind(&ThreadWatcherList::SetStopped, false));

  if (!WatchDogThread::PostDelayedTask(
          FROM_HERE,
          base::Bind(&ThreadWatcherList::InitializeAndStartWatching,
                     unresponsive_threshold,
                     crash_on_hang_threads),
          base::TimeDelta::FromSeconds(g_initialize_delay_seconds))) {
    StartupTimeBomb::DisarmStartupTimeBomb();
  }
}
