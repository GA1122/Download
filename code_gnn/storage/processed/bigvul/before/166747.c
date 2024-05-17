Address BaseArena::LazySweep(size_t allocation_size, size_t gc_info_index) {
  if (SweepingCompleted())
    return nullptr;

  CHECK(GetThreadState()->IsSweepingInProgress());

  if (GetThreadState()->SweepForbidden())
    return nullptr;

  Address result = nullptr;
  {
    ThreadHeapStatsCollector::Scope stats_scope(
        GetThreadState()->Heap().stats_collector(),
        ThreadHeapStatsCollector::kLazySweepOnAllocation);
    ThreadState::SweepForbiddenScope sweep_forbidden(GetThreadState());
    ScriptForbiddenScope script_forbidden;
    result = LazySweepPages(allocation_size, gc_info_index);
  }
  ThreadHeap::ReportMemoryUsageForTracing();
  return result;
}
