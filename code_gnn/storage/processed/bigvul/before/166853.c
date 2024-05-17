  ~IncrementalMarkingScope() {
    EXPECT_TRUE(marking_worklist_->IsGlobalEmpty());
    EXPECT_TRUE(not_fully_constructed_worklist_->IsGlobalEmpty());
    thread_state_->DisableIncrementalMarkingBarrier();
    heap_.GetWeakCallbackWorklist()->Clear();
    thread_state_->SetGCPhase(ThreadState::GCPhase::kSweeping);
    thread_state_->SetGCPhase(ThreadState::GCPhase::kNone);
  }
