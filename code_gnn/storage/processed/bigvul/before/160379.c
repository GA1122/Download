bool BaseArena::lazySweepWithDeadline(double deadlineSeconds) {
  static const int deadlineCheckInterval = 10;

  RELEASE_ASSERT(getThreadState()->isSweepingInProgress());
  ASSERT(getThreadState()->sweepForbidden());
  ASSERT(!getThreadState()->isMainThread() ||
         ScriptForbiddenScope::isScriptForbidden());

  NormalPageArena* normalArena = nullptr;
  if (m_firstUnsweptPage && !m_firstUnsweptPage->isLargeObjectPage()) {
    NormalPage* normalPage = reinterpret_cast<NormalPage*>(m_firstUnsweptPage);
    normalArena = normalPage->arenaForNormalPage();
    normalArena->setIsLazySweeping(true);
  }
  int pageCount = 1;
  while (m_firstUnsweptPage) {
    sweepUnsweptPage();
    if (pageCount % deadlineCheckInterval == 0) {
      if (deadlineSeconds <= monotonicallyIncreasingTime()) {
        ThreadHeap::reportMemoryUsageForTracing();
        if (normalArena)
          normalArena->setIsLazySweeping(false);
        return !m_firstUnsweptPage;
      }
    }
    pageCount++;
  }
  ThreadHeap::reportMemoryUsageForTracing();
  if (normalArena)
    normalArena->setIsLazySweeping(false);
  return true;
}
