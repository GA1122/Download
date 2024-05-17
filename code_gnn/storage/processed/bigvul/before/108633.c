  void Reset() {
    state_ = NOT_BLOCKED;
    callback_.Reset();
    auth_callback_.Reset();
  }
