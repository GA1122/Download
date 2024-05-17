void RenderWidgetHostImpl::WasShown(bool record_presentation_time) {
  if (!is_hidden_)
    return;

  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::WasShown");
  is_hidden_ = false;

  ForceFirstFrameAfterNavigationTimeout();

  SendScreenRects();
  RestartInputEventAckTimeoutIfNecessary();

  bool needs_repainting = true;
  needs_repainting_on_restore_ = false;
  Send(new ViewMsg_WasShown(
      routing_id_, needs_repainting,
      record_presentation_time ? base::TimeTicks::Now() : base::TimeTicks()));

  process_->UpdateClientPriority(this);

  bool is_visible = true;
  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_VISIBILITY_CHANGED,
      Source<RenderWidgetHost>(this),
      Details<bool>(&is_visible));
  for (auto& observer : observers_)
    observer.RenderWidgetHostVisibilityChanged(this, true);

  SynchronizeVisualProperties();
}
