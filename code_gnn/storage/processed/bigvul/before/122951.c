void RenderWidgetHostImpl::Destroy() {
  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_HOST_DESTROYED,
      Source<RenderWidgetHost>(this),
      NotificationService::NoDetails());

  if (view_)
    view_->Destroy();

  delete this;
}
