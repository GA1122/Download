void ChildThread::OnProcessFinalRelease() {
  if (on_channel_error_called_) {
    base::MessageLoop::current()->Quit();
    return;
  }

  Send(new ChildProcessHostMsg_ShutdownRequest);
}
