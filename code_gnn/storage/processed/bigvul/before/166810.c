  explicit IncrementalMarkingScope(ThreadState* thread_state)
      : IncrementalMarkingScopeBase(thread_state),
        gc_forbidden_scope_(thread_state),
        marking_worklist_(heap_.GetMarkingWorklist()),
        not_fully_constructed_worklist_(
            heap_.GetNotFullyConstructedWorklist()) {
    thread_state_->SetGCPhase(ThreadState::GCPhase::kMarking);
    ThreadState::AtomicPauseScope atomic_pause_scope_(thread_state_);
    EXPECT_TRUE(marking_worklist_->IsGlobalEmpty());
    EXPECT_TRUE(not_fully_constructed_worklist_->IsGlobalEmpty());
    thread_state->EnableIncrementalMarkingBarrier();
    thread_state->current_gc_data_.visitor =
        MarkingVisitor::Create(thread_state, MarkingVisitor::kGlobalMarking);
  }
