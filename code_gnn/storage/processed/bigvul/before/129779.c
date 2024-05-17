void QuitMainThreadMessageLoop() {
  base::MessageLoop::current()->Quit();
}
