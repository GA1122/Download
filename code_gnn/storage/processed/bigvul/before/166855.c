  ~IncrementalMarkingTestDriver() {
    if (thread_state_->IsIncrementalMarking())
      FinishGC();
  }
