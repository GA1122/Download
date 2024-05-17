scoped_ptr<DesktopSession> MockDaemonProcess::DoCreateDesktopSession(
    int terminal_id) {
  return scoped_ptr<DesktopSession>(DoCreateDesktopSessionPtr(terminal_id));
}
