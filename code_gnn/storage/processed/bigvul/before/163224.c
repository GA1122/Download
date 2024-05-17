  virtual void DidCallWillStartRequest() {
    will_start_called_++;
    if (will_start_loop_runner_)
      will_start_loop_runner_->Quit();
  }
