ShutdownWatcherHelper::ShutdownWatcherHelper()
    : shutdown_watchdog_(nullptr),
      thread_id_(base::PlatformThread::CurrentId()) {
}
