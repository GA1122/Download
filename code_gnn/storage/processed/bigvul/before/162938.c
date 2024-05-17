void MockCoordinator::RequestGlobalMemoryDumpAndAppendToTrace(
    MemoryDumpType dump_type,
    MemoryDumpLevelOfDetail level_of_detail,
    const RequestGlobalMemoryDumpAndAppendToTraceCallback& callback) {
  client_->RequestChromeDump(dump_type, level_of_detail);
  callback.Run(1, true);
}
