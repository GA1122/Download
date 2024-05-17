void WebKitTestController::WebContentsDestroyed() {
  DCHECK(CalledOnValidThread());
  printer_->AddErrorMessage("FAIL: main window was destroyed");
  DiscardMainWindow();
}
