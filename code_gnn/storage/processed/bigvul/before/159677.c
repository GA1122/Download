  void Wait() {
    if (deleted_)
      return;

    message_loop_runner_->Run();
  }
