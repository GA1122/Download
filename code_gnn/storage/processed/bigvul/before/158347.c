void RenderWidgetHostImpl::RendererIsUnresponsive(
    base::RepeatingClosure restart_hang_monitor_timeout) {
  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_HOST_HANG,
      Source<RenderWidgetHost>(this),
      NotificationService::NoDetails());
  is_unresponsive_ = true;

  if (delegate_) {
    delegate_->RendererUnresponsive(this,
                                    std::move(restart_hang_monitor_timeout));
  }

}
