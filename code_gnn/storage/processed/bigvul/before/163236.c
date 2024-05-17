  void WaitForThrottleWillProcess() {
    if (will_process_called_)
      return;
    will_process_loop_runner_ = new MessageLoopRunner();
    will_process_loop_runner_->Run();
    will_process_loop_runner_ = nullptr;
  }
