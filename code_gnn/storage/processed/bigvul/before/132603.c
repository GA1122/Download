void WebKitTestController::RenderProcessGone(base::TerminationStatus status) {
  DCHECK(CalledOnValidThread());
  if (current_pid_ != base::kNullProcessId) {
    printer_->AddErrorMessage(std::string("#CRASHED - renderer (pid ") +
                              base::IntToString(current_pid_) + ")");
  } else {
    printer_->AddErrorMessage("#CRASHED - renderer");
  }
  DiscardMainWindow();
}
