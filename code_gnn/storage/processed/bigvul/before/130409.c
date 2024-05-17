bool ThreadWatcher::IsVeryUnresponsive() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  return unresponsive_count_ >= unresponsive_threshold_;
}
