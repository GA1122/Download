ThreadWatcherObserver::ThreadWatcherObserver(
    const base::TimeDelta& wakeup_interval)
    : last_wakeup_time_(base::TimeTicks::Now()),
      wakeup_interval_(wakeup_interval) {
  CHECK(!g_thread_watcher_observer_);
  g_thread_watcher_observer_ = this;
}
