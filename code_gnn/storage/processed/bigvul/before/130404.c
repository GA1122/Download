void WatchDogThread::Init() {
  base::ThreadRestrictions::SetIOAllowed(false);

  base::AutoLock lock(g_watchdog_lock.Get());
  CHECK(!g_watchdog_thread);
  g_watchdog_thread = this;
}
