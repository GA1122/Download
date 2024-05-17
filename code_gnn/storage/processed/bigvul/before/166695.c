void ThreadHeap::MarkNotFullyConstructedObjects(Visitor* visitor) {
  TRACE_EVENT0("blink_gc", "ThreadHeap::MarkNotFullyConstructedObjects");
  DCHECK(!thread_state_->IsIncrementalMarking());

  NotFullyConstructedItem item;
  while (
      not_fully_constructed_worklist_->Pop(WorklistTaskId::MainThread, &item)) {
    BasePage* const page = PageFromObject(item);
    reinterpret_cast<MarkingVisitor*>(visitor)->ConservativelyMarkAddress(
        page, reinterpret_cast<Address>(item));
  }
}
