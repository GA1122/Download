  void DidCreateOrOpen(File::Error error) {
    error_ = error;
    MessageLoop::current()->QuitWhenIdle();
  }
