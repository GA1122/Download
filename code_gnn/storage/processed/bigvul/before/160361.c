void BaseArena::completeSweep() {
  RELEASE_ASSERT(getThreadState()->isSweepingInProgress());
  ASSERT(getThreadState()->sweepForbidden());
  ASSERT(!getThreadState()->isMainThread() ||
         ScriptForbiddenScope::isScriptForbidden());

  while (m_firstUnsweptPage) {
    sweepUnsweptPage();
  }
  ThreadHeap::reportMemoryUsageForTracing();
}
