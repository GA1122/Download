void MemoryInstrumentation::RequestGlobalDumpAndAppendToTrace(
    MemoryDumpType dump_type,
    MemoryDumpLevelOfDetail level_of_detail,
    RequestGlobalMemoryDumpAndAppendToTraceCallback callback) {
  const auto& coordinator = GetCoordinatorBindingForCurrentThread();
  coordinator->RequestGlobalMemoryDumpAndAppendToTrace(
       dump_type, level_of_detail, callback);
 }
