void ThreadHeap::RegisterWeakTable(void* table,
                                   EphemeronCallback iteration_callback) {
  DCHECK(thread_state_->InAtomicMarkingPause());
#if DCHECK_IS_ON()
  auto result = ephemeron_callbacks_.insert(table, iteration_callback);
  DCHECK(result.is_new_entry ||
         result.stored_value->value == iteration_callback);
#else
  ephemeron_callbacks_.insert(table, iteration_callback);
#endif   
}
