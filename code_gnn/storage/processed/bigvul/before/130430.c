void ThreadWatcher::StartWatching(const WatchingParams& params) {
  DCHECK_GE(params.sleep_time.InMilliseconds(), 0);
  DCHECK_GE(params.unresponsive_time.InMilliseconds(),
            params.sleep_time.InMilliseconds());

  if (!WatchDogThread::CurrentlyOnWatchDogThread()) {
    WatchDogThread::PostTask(
        FROM_HERE,
        base::Bind(&ThreadWatcher::StartWatching, params));
    return;
  }

  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());

  ThreadWatcher* watcher = new ThreadWatcher(params);

  DCHECK(watcher);
  if (!ThreadWatcherList::IsRegistered(params.thread_id))
    return;
  watcher->ActivateThreadWatching();
}
