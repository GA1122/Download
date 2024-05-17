void RenderWidgetHostImpl::WasHidden() {
  if (is_hidden_)
    return;

  RejectMouseLockOrUnlockIfNecessary();

  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::WasHidden");
  is_hidden_ = true;

  visual_properties_ack_pending_ = false;

  StopInputEventAckTimeout();

  Send(new ViewMsg_WasHidden(routing_id_));

  process_->UpdateClientPriority(this);

  bool is_visible = false;
  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_VISIBILITY_CHANGED,
      Source<RenderWidgetHost>(this),
      Details<bool>(&is_visible));
  for (auto& observer : observers_)
    observer.RenderWidgetHostVisibilityChanged(this, false);
}
