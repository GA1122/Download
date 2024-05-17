  explicit IncrementalMarkingScopeBase(ThreadState* thread_state)
      : thread_state_(thread_state), heap_(thread_state_->Heap()) {
    heap_.CommitCallbackStacks();
  }
