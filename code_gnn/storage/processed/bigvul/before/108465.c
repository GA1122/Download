  bool AddCallback(const Callback& callback) {
    callbacks_.push_back(callback);
    return (callbacks_.size() == 1);
  }
