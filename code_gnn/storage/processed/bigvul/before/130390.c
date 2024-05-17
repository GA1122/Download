void StartupTimeBomb::Arm(const base::TimeDelta& duration) {
  DCHECK_EQ(thread_id_, base::PlatformThread::CurrentId());
  DCHECK(!startup_watchdog_);
  startup_watchdog_ = new StartupWatchDogThread(duration);
  startup_watchdog_->Arm();
  return;
}
