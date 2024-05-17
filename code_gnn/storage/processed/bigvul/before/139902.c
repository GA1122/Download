  std::string TakeLog() {
    std::string result;
    result.swap(log_);
    return result;
  }
