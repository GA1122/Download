void RenderProcessHostImpl::OnProcessLaunched() {
  if (deleting_soon_)
    return;

  if (child_process_launcher_.get()) {
    if (!child_process_launcher_->GetHandle()) {
      OnChannelError();
      return;
    }

    child_process_launcher_->SetProcessBackgrounded(backgrounded_);
  }

  NotificationService::current()->Notify(
      NOTIFICATION_RENDERER_PROCESS_CREATED,
      Source<RenderProcessHost>(this),
      NotificationService::NoDetails());

  while (!queued_messages_.empty()) {
    Send(queued_messages_.front());
    queued_messages_.pop();
  }
}
