  void WaitForThrottleWillStart() {
    if (will_start_called_)
      return;
    will_start_loop_runner_ = new MessageLoopRunner();
    will_start_loop_runner_->Run();
    will_start_loop_runner_ = nullptr;
  }
