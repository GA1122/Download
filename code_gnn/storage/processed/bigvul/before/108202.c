  explicit CancelableQuitTask(const std::string& timeout_message)
      : timeout_message_(timeout_message),
        cancelled_(false) {
  }
