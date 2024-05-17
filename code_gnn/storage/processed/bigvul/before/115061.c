void TestingAutomationProvider::WaitForProcessLauncherThreadToGoIdle(
    IPC::Message* reply_message) {
  new WaitForProcessLauncherThreadToGoIdleObserver(this, reply_message);
}
