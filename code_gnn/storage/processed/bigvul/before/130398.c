void StartupTimeBomb::Disarm() {
  DCHECK_EQ(thread_id_, base::PlatformThread::CurrentId());
  if (startup_watchdog_) {
    startup_watchdog_->Disarm();
    startup_watchdog_->Cleanup();
    DeleteStartupWatchdog();
  }
}
