void AudioOutputDeviceTest::WaitUntilRenderCallback() {
  io_loop_.PostDelayedTask(FROM_HERE, MessageLoop::QuitClosure(),
                           TestTimeouts::action_timeout());
  io_loop_.Run();
}
