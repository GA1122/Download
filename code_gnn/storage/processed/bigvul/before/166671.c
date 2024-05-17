bool ThreadHeap::AdvanceMarkingStackProcessing(Visitor* visitor,
                                               double deadline_seconds) {
  const size_t kDeadlineCheckInterval = 2500;
  size_t processed_callback_count = 0;
  do {
    {
      TRACE_EVENT0("blink_gc", "ThreadHeap::processMarkingStackSingleThreaded");
      MarkingItem item;
      while (marking_worklist_->Pop(WorklistTaskId::MainThread, &item)) {
        item.callback(visitor, item.object);
        processed_callback_count++;
        if (processed_callback_count % kDeadlineCheckInterval == 0) {
          if (deadline_seconds <= CurrentTimeTicksInSeconds()) {
            return false;
          }
        }
      }
    }

    InvokeEphemeronCallbacks(visitor);

  } while (!marking_worklist_->IsGlobalEmpty());
  return true;
}
