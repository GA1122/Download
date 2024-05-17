Address BaseArena::lazySweep(size_t allocationSize, size_t gcInfoIndex) {
  if (!m_firstUnsweptPage)
    return nullptr;

  RELEASE_ASSERT(getThreadState()->isSweepingInProgress());

  if (getThreadState()->sweepForbidden())
    return nullptr;

  TRACE_EVENT0("blink_gc", "BaseArena::lazySweepPages");
  ThreadState::SweepForbiddenScope sweepForbidden(getThreadState());
  ScriptForbiddenIfMainThreadScope scriptForbidden;

  double startTime = WTF::currentTimeMS();
  Address result = lazySweepPages(allocationSize, gcInfoIndex);
  getThreadState()->accumulateSweepingTime(WTF::currentTimeMS() - startTime);
  ThreadHeap::reportMemoryUsageForTracing();

  return result;
}
