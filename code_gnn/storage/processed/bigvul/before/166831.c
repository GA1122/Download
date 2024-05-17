  void Start() {
    thread_state_->ScheduleIncrementalMarkingStart();
    thread_state_->RunScheduledGC(BlinkGC::kNoHeapPointersOnStack);
  }
