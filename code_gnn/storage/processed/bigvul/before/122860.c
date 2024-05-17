void RenderProcessHostImpl::Cleanup() {
  if (render_widget_hosts_.IsEmpty()) {
    DCHECK_EQ(0, pending_views_);
    NotificationService::current()->Notify(
        NOTIFICATION_RENDERER_PROCESS_TERMINATED,
        Source<RenderProcessHost>(this),
        NotificationService::NoDetails());
    MessageLoop::current()->DeleteSoon(FROM_HERE, this);
    deleting_soon_ = true;
    channel_.reset();
    gpu_message_filter_ = NULL;

    UnregisterHost(GetID());
  }
}
