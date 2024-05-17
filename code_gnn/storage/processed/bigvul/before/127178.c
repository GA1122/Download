void DaemonProcess::DeleteAllDesktopSessions() {
  while (!desktop_sessions_.empty()) {
    delete desktop_sessions_.front();
    desktop_sessions_.pop_front();
  }
}
