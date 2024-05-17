void WebRTCAudioDeviceTest::TearDown() {
  SetAudioUtilCallback(NULL);

  ChildProcess::current()->main_thread()->message_loop()->RunAllPending();

  ChildProcess::current()->io_message_loop()->PostTask(FROM_HERE,
      base::Bind(&WebRTCAudioDeviceTest::DestroyChannel,
                 base::Unretained(this)));
  WaitForIOThreadCompletion();

  WaitForAudioManagerCompletion();

  ChildProcess::current()->io_message_loop()->PostTask(FROM_HERE,
      base::Bind(&WebRTCAudioDeviceTest::UninitializeIOThread,
                 base::Unretained((this))));
  WaitForIOThreadCompletion();
  mock_process_.reset();
}
