ShutdownWatcherHelper::~ShutdownWatcherHelper() {
  DCHECK_EQ(thread_id_, base::PlatformThread::CurrentId());
  if (shutdown_watchdog_) {
    shutdown_watchdog_->Disarm();
    delete shutdown_watchdog_;
    shutdown_watchdog_ = nullptr;
  }
}
