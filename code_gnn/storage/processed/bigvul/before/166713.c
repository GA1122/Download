void ThreadHeap::VisitPersistentRoots(Visitor* visitor) {
  DCHECK(thread_state_->InAtomicMarkingPause());
  TRACE_EVENT0("blink_gc", "ThreadHeap::visitPersistentRoots");
  thread_state_->VisitPersistents(visitor);
}
