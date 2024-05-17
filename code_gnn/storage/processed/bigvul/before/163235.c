  void WaitForThrottleWillFail() {
    if (will_fail_called_)
      return;
    will_fail_loop_runner_ = new MessageLoopRunner();
    will_fail_loop_runner_->Run();
    will_fail_loop_runner_ = nullptr;
  }
