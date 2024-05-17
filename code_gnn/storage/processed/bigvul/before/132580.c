void WebKitTestController::OnLeakDetectionDone(
    const LeakDetectionResult& result) {
  if (!result.leaked) {
    base::MessageLoop::current()->PostTask(FROM_HERE,
                                           base::MessageLoop::QuitClosure());
    return;
  }

  printer_->AddErrorMessage(
      base::StringPrintf("#LEAK - renderer pid %d (%s)", current_pid_,
                         result.detail.c_str()));
  CHECK(!crash_when_leak_found_);

  DiscardMainWindow();
}
