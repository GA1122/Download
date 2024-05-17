  void RunOnError(const base::Closure& callback) {
    error_callback_ = callback;
    waiter_.WaitForEvent(EVENT_ERROR);
  }
