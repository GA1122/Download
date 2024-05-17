void WorkerProcessLauncherTest::QuitMainMessageLoop() {
  message_loop_.PostTask(FROM_HERE, MessageLoop::QuitClosure());
}
