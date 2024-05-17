void RenderProcessHostWatcher::RenderProcessExited(
    RenderProcessHost* host,
    const ChildProcessTerminationInfo& info) {
  did_exit_normally_ =
      info.status == base::TERMINATION_STATUS_NORMAL_TERMINATION;
  if (type_ == WATCH_FOR_PROCESS_EXIT)
    message_loop_runner_->Quit();
}
