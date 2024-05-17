void RenderProcessHostImpl::ProcessDied(bool already_dead,
                                        RendererClosedDetails* known_details) {

  DCHECK(!within_process_died_observer_);

  DCHECK(!deleting_soon_);

  base::TerminationStatus status = base::TERMINATION_STATUS_NORMAL_TERMINATION;
  int exit_code = 0;
  if (known_details) {
    status = known_details->status;
    exit_code = known_details->exit_code;
  } else if (child_process_launcher_.get()) {
    status = child_process_launcher_->GetChildTerminationStatus(already_dead,
                                                                &exit_code);
    if (already_dead && status == base::TERMINATION_STATUS_STILL_RUNNING) {
      status = base::TERMINATION_STATUS_PROCESS_CRASHED;
    }
  }

  RendererClosedDetails details(status, exit_code);

  child_process_launcher_.reset();
  is_dead_ = true;
  if (route_provider_binding_.is_bound())
    route_provider_binding_.Close();
  ResetChannelProxy();

  UpdateProcessPriority();
  DCHECK(!is_process_backgrounded_);

  within_process_died_observer_ = true;
  NotificationService::current()->Notify(
      NOTIFICATION_RENDERER_PROCESS_CLOSED, Source<RenderProcessHost>(this),
      Details<RendererClosedDetails>(&details));
  for (auto& observer : observers_)
    observer.RenderProcessExited(this, status, exit_code);
  within_process_died_observer_ = false;

  message_port_message_filter_ = NULL;

  RemoveUserData(kSessionStorageHolderKey);

  IDMap<IPC::Listener*>::iterator iter(&listeners_);
  while (!iter.IsAtEnd()) {
    iter.GetCurrentValue()->OnMessageReceived(FrameHostMsg_RenderProcessGone(
        iter.GetCurrentKey(), static_cast<int>(status), exit_code));
    iter.Advance();
  }

  EnableSendQueue();

  if (delayed_cleanup_needed_)
    Cleanup();

}
