void DOMMessageQueue::RenderProcessGone(base::TerminationStatus status) {
  VLOG(0) << "DOMMessageQueue::RenderProcessGone " << status;
  switch (status) {
    case base::TERMINATION_STATUS_NORMAL_TERMINATION:
    case base::TERMINATION_STATUS_STILL_RUNNING:
      break;
    default:
      renderer_crashed_ = true;
      if (message_loop_runner_.get())
        message_loop_runner_->Quit();
      break;
  }
}
