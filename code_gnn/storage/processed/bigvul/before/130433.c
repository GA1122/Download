bool WatchDogThread::Started() const {
  base::AutoLock lock(g_watchdog_lock.Get());
  return g_watchdog_thread != nullptr;
}
