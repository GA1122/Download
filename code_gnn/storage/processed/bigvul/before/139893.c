  void Finalize() {
    manager_->EndSession(client_.get());
  }
