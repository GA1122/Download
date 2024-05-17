void ThreadWatcherObserver::OnUserActivityDetected() {
  base::TimeTicks now = base::TimeTicks::Now();
  if ((now - last_wakeup_time_) < wakeup_interval_)
    return;
  last_wakeup_time_ = now;
  WatchDogThread::PostTask(
      FROM_HERE,
      base::Bind(&ThreadWatcherList::WakeUpAll));
}
