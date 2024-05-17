void ChildThread::OnShutdown() {
  base::MessageLoop::current()->Quit();
}
