void WebKitTestController::WorkerCrashed() {
  DCHECK(CalledOnValidThread());
  printer_->AddErrorMessage("#CRASHED - worker");
  DiscardMainWindow();
}
