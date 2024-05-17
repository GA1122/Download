  void FinishSteps() {
    CHECK(thread_state_->IsIncrementalMarking());
    while (SingleStep()) {
    }
  }
