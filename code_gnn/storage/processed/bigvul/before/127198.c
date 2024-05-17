void DaemonProcessTest::TearDown() {
  daemon_process_->Stop();
  message_loop_.Run();
}
