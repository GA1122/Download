  void Initialize() {
    client_.reset(new FakeMidiManagerClient(&logger_));
    manager_->StartSession(client_.get());
  }
