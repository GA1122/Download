void ChildThread::OnChannelError() {
  set_on_channel_error_called(true);
  base::MessageLoop::current()->Quit();
}
