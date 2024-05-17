  void WaitForThrottleWillRedirect() {
    if (will_redirect_called_)
      return;
    will_redirect_loop_runner_ = new MessageLoopRunner();
    will_redirect_loop_runner_->Run();
    will_redirect_loop_runner_ = nullptr;
  }
