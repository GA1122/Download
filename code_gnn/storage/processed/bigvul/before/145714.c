  void NotifyFail(const std::string& message) {
    FinishTesting();
    FAIL() << logs_ << message;
  }
