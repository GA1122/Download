void ThreadHeap::VisitStackRoots(MarkingVisitor* visitor) {
  DCHECK(thread_state_->InAtomicMarkingPause());
  TRACE_EVENT0("blink_gc", "ThreadHeap::visitStackRoots");
  address_cache_->FlushIfDirty();
  address_cache_->EnableLookup();
  thread_state_->VisitStack(visitor);
  address_cache_->DisableLookup();
}
