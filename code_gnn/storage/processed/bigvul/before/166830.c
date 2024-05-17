  bool SingleStep() {
    CHECK(thread_state_->IsIncrementalMarking());
    if (thread_state_->GcState() ==
        ThreadState::kIncrementalMarkingStepScheduled) {
      thread_state_->RunScheduledGC(BlinkGC::kNoHeapPointersOnStack);
      return true;
    }
    return false;
  }
