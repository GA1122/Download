void ThreadWatcher::ResetHangCounters() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  unresponsive_count_ = 0;
  hung_processing_complete_ = false;
}
