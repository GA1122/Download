void MemoryInstrumentation::RequestGlobalDumpForPid(
    base::ProcessId pid,
    RequestGlobalDumpCallback callback) {
  const auto& coordinator = GetCoordinatorBindingForCurrentThread();
  coordinator->RequestGlobalMemoryDumpForPid(pid, callback);
}
