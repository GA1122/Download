  bool IsStartable() const {
    return !started_ && !callbacks_.empty();
  }
