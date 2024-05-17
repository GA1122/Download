void RenderThread::ScheduleIdleHandler(double initial_delay_s) {
  idle_notification_delay_in_s_ = initial_delay_s;
  idle_timer_.Stop();
  idle_timer_.Start(
      base::TimeDelta::FromSeconds(static_cast<int64>(initial_delay_s)),
      this, &RenderThread::IdleHandler);
}
