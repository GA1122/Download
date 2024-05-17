void RenderProcessHostImpl::ProcessDied(bool already_dead) {

  int exit_code = 0;
  base::TerminationStatus status =
      child_process_launcher_.get() ?
      child_process_launcher_->GetChildTerminationStatus(already_dead,
                                                         &exit_code) :
      base::TERMINATION_STATUS_NORMAL_TERMINATION;

  RendererClosedDetails details(GetHandle(), status, exit_code);
  NotificationService::current()->Notify(
      NOTIFICATION_RENDERER_PROCESS_CLOSED,
      Source<RenderProcessHost>(this),
      Details<RendererClosedDetails>(&details));

  child_process_launcher_.reset();
  channel_.reset();
  gpu_message_filter_ = NULL;

  IDMap<RenderWidgetHost>::iterator iter(&render_widget_hosts_);
  while (!iter.IsAtEnd()) {
    RenderWidgetHostImpl::From(iter.GetCurrentValue())->OnMessageReceived(
        ViewHostMsg_RenderViewGone(iter.GetCurrentKey(),
                                   static_cast<int>(status),
                                   exit_code));
    iter.Advance();
  }

  ClearTransportDIBCache();

}
