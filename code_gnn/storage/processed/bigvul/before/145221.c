void Dispatcher::IdleNotification() {
  if (set_idle_notifications_ && forced_idle_timer_) {
    int64_t forced_delay_ms =
        std::max(RenderThread::Get()->GetIdleNotificationDelayInMs(),
                 kMaxExtensionIdleHandlerDelayMs);
    forced_idle_timer_->Stop();
    forced_idle_timer_->Start(
        FROM_HERE,
        base::TimeDelta::FromMilliseconds(forced_delay_ms),
        RenderThread::Get(),
        &RenderThread::IdleHandler);
  }
}
