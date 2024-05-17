  void WaitForEvents(size_t numbers_of_events) {
    while (records_.size() < numbers_of_events) {
      loop_.reset(new base::RunLoop);
      loop_->Run();
      loop_.reset();
    }
  }
