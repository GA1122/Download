StartupTimeBomb::StartupTimeBomb()
    : startup_watchdog_(nullptr),
      thread_id_(base::PlatformThread::CurrentId()) {
  CHECK(!g_startup_timebomb_);
  g_startup_timebomb_ = this;
}
