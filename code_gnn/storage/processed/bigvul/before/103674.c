void BrowserRenderProcessHost::OnChannelError() {
  if (!channel_.get())
    return;

  int exit_code = 0;
  base::TerminationStatus status =
      child_process_launcher_.get() ?
      child_process_launcher_->GetChildTerminationStatus(&exit_code) :
      base::TERMINATION_STATUS_NORMAL_TERMINATION;

  if (status == base::TERMINATION_STATUS_PROCESS_CRASHED ||
      status == base::TERMINATION_STATUS_ABNORMAL_TERMINATION) {
    UMA_HISTOGRAM_PERCENTAGE("BrowserRenderProcessHost.ChildCrashes",
                             is_extension_process_ ? 2 : 1);
  }

  if (status == base::TERMINATION_STATUS_PROCESS_WAS_KILLED) {
    UMA_HISTOGRAM_PERCENTAGE("BrowserRenderProcessHost.ChildKills",
                             is_extension_process_ ? 2 : 1);
  }

  RendererClosedDetails details(status, exit_code, is_extension_process_);
  NotificationService::current()->Notify(
      content::NOTIFICATION_RENDERER_PROCESS_CLOSED,
      Source<RenderProcessHost>(this),
      Details<RendererClosedDetails>(&details));

  child_process_launcher_.reset();
  channel_.reset();

  IDMap<IPC::Channel::Listener>::iterator iter(&listeners_);
  while (!iter.IsAtEnd()) {
    iter.GetCurrentValue()->OnMessageReceived(
        ViewHostMsg_RenderViewGone(iter.GetCurrentKey(),
                                   static_cast<int>(status),
                                   exit_code));
    iter.Advance();
  }

  ClearTransportDIBCache();

}
