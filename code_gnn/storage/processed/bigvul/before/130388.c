void ThreadWatcher::ActivateThreadWatching() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  if (active_) return;
  active_ = true;
  ping_count_ = unresponsive_threshold_;
  ResetHangCounters();
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::Bind(&ThreadWatcher::PostPingMessage,
                            weak_ptr_factory_.GetWeakPtr()));
}
