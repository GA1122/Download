void SandboxIPCHandler::SetObserverForTests(
    SandboxIPCHandler::TestObserver* observer) {
  g_test_observer = observer;
}
