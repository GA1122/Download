void DaemonProcessTest::QuitMessageLoop() {
  message_loop_.PostTask(FROM_HERE, MessageLoop::QuitClosure());
}
