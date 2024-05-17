void BaseArena::CompleteSweep() {
  CHECK(GetThreadState()->IsSweepingInProgress());
  DCHECK(GetThreadState()->SweepForbidden());
  DCHECK(ScriptForbiddenScope::IsScriptForbidden());

  MakeIterable();

  while (!SweepingCompleted()) {
    SweepUnsweptPage();
  }
  ThreadHeap::ReportMemoryUsageForTracing();
}
