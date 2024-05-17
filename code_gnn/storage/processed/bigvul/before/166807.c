  void FinishGC() {
    CHECK(thread_state_->IsIncrementalMarking());
    FinishSteps();
    CHECK_EQ(ThreadState::kIncrementalMarkingFinalizeScheduled,
             thread_state_->GcState());
    thread_state_->RunScheduledGC(BlinkGC::kNoHeapPointersOnStack);
    CHECK(!thread_state_->IsIncrementalMarking());
    thread_state_->CompleteSweep();
  }
