  virtual ~QueueTouchEventDelegate() {
    while(!queue_.empty()) {
      delete queue_.front();
      queue_.pop();
    }
  }
