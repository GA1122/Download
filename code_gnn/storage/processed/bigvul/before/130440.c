void ThreadWatcher::WakeUp() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  if (!active_) return;

  unresponsive_count_ = 0;

  if (ping_count_ <= 0) {
    ping_count_ = unresponsive_threshold_;
    ResetHangCounters();
    PostPingMessage();
  } else {
    ping_count_ = unresponsive_threshold_;
  }
}
