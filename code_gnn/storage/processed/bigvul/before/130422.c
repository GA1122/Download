bool WatchDogThread::PostTaskHelper(
    const tracked_objects::Location& from_here,
    const base::Closure& task,
    base::TimeDelta delay) {
  {
    base::AutoLock lock(g_watchdog_lock.Get());

    base::MessageLoop* message_loop = g_watchdog_thread ?
        g_watchdog_thread->message_loop() : nullptr;
    if (message_loop) {
      message_loop->task_runner()->PostDelayedTask(from_here, task, delay);
      return true;
    }
  }

  return false;
}
