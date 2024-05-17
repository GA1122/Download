  virtual void DidCallWillRedirectRequest() {
    will_redirect_called_++;
    if (will_redirect_loop_runner_)
      will_redirect_loop_runner_->Quit();
  }
