  virtual void StartAsync() {
    request_->Cancel();
    this->NotifyRestartRequired();
  }
