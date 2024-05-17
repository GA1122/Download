  void DoCallback(int rv) {
    ASSERT_NE(NOT_BLOCKED, state_);
    CompletionCallback callback = callback_;
    Reset();
    callback.Run(rv);
  }
