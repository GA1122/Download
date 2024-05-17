  virtual void DidCallWillFailRequest() {
    will_fail_called_++;
    if (will_fail_loop_runner_)
      will_fail_loop_runner_->Quit();
  }
