bool WatchDogThread::CurrentlyOnWatchDogThread() {
  base::AutoLock lock(g_watchdog_lock.Get());
  return g_watchdog_thread &&
      g_watchdog_thread->message_loop() == base::MessageLoop::current();
}
