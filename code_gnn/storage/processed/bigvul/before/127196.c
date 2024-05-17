void DaemonProcessTest::SetUp() {
  scoped_refptr<AutoThreadTaskRunner> task_runner = new AutoThreadTaskRunner(
      message_loop_.message_loop_proxy(),
      base::Bind(&DaemonProcessTest::QuitMessageLoop,
                 base::Unretained(this)));
  daemon_process_.reset(
      new MockDaemonProcess(task_runner, task_runner,
                            base::Bind(&DaemonProcessTest::DeleteDaemonProcess,
                                       base::Unretained(this))));

  EXPECT_CALL(*daemon_process_, DoCreateDesktopSessionPtr(_))
      .Times(AnyNumber())
      .WillRepeatedly(Invoke(this, &DaemonProcessTest::DoCreateDesktopSession));
  EXPECT_CALL(*daemon_process_, LaunchNetworkProcess())
      .Times(AnyNumber())
      .WillRepeatedly(Invoke(this, &DaemonProcessTest::LaunchNetworkProcess));
}
