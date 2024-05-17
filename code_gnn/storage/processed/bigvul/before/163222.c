  virtual void DidCallWillProcessResponse() {
    will_process_called_++;
    if (will_process_loop_runner_)
      will_process_loop_runner_->Quit();
  }
