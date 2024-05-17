  void Wait() {
    if (!scroll_end_received_) {
      message_loop_runner_->Run();
    }
  }
