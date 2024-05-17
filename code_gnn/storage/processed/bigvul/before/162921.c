void MemoryInstrumentation::RequestGlobalDump(
    const std::vector<std::string>& allocator_dump_names,
    RequestGlobalDumpCallback callback) {
  const auto& coordinator = GetCoordinatorBindingForCurrentThread();
  coordinator->RequestGlobalMemoryDump(MemoryDumpType::SUMMARY_ONLY,
                                       MemoryDumpLevelOfDetail::BACKGROUND,
                                       allocator_dump_names, callback);
}
