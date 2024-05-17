void RenderWidgetHostImpl::CheckRendererIsUnresponsive() {
  if (time_when_considered_hung_.is_null())
    return;

  Time now = Time::Now();
  if (now < time_when_considered_hung_) {
    StartHangMonitorTimeout(time_when_considered_hung_ - now);
    return;
  }

  NotificationService::current()->Notify(
      NOTIFICATION_RENDERER_PROCESS_HANG,
      Source<RenderWidgetHost>(this),
      NotificationService::NoDetails());
  is_unresponsive_ = true;
  NotifyRendererUnresponsive();
}
