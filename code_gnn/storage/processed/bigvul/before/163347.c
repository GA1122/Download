void RenderThreadImpl::IdleHandler() {
  bool run_in_foreground_tab = (widget_count_ > hidden_widget_count_) &&
                               GetContentClient()->renderer()->
                                   RunIdleHandlerWhenWidgetsHidden();
  if (run_in_foreground_tab) {
    if (idle_notifications_to_skip_ > 0) {
      --idle_notifications_to_skip_;
    } else {
      ReleaseFreeMemory();
    }
    ScheduleIdleHandler(kLongIdleHandlerDelayMs);
    return;
  }

  ReleaseFreeMemory();

  bool continue_timer = !webkit_shared_timer_suspended_;

  if (continue_timer) {
    ScheduleIdleHandler(
        std::max(kLongIdleHandlerDelayMs,
                 idle_notification_delay_in_ms_ +
                 1000000 / (idle_notification_delay_in_ms_ + 2000)));

  } else {
    idle_timer_.Stop();
  }

  for (auto& observer : observers_)
    observer.IdleNotification();
}
