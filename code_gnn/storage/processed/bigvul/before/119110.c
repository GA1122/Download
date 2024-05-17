  void DidFinish(File::Error error) {
    error_ = error;
    MessageLoop::current()->QuitWhenIdle();
  }
