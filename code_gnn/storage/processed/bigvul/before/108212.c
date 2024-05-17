  virtual void Run() {
    if (cancelled_) {
      return;
    }
    FAIL() << timeout_message_;
    MessageLoop::current()->Quit();
  }
