void WebRTCAudioDeviceTest::WaitForIOThreadCompletion() {
  WaitForMessageLoopCompletion(
      ChildProcess::current()->io_message_loop()->message_loop_proxy());
}
