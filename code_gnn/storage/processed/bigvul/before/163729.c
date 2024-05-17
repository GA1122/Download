  void WaitUntilShown() {
    while (!is_visible_) {
      message_loop_runner_ = new MessageLoopRunner;
      message_loop_runner_->Run();
    }
  }
