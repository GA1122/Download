  void clear_network_error_logging_service() {
    session_deps_.network_error_logging_service.reset();
    test_network_error_logging_service_ = nullptr;
  }
