  void OnTimeout() {
    LOG_IF(ERROR, is_removing_) << "Timed out";
    SignalDone();
  }
