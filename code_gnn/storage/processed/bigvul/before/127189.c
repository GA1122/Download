DesktopSession* DaemonProcessTest::DoCreateDesktopSession(int terminal_id) {
  return new FakeDesktopSession(daemon_process_.get(), terminal_id);
}
