void MockCoordinator::RequestGlobalMemoryDump(
    MemoryDumpType dump_type,
    MemoryDumpLevelOfDetail level_of_detail,
    const std::vector<std::string>& allocator_dump_names,
    const RequestGlobalMemoryDumpCallback& callback) {
  client_->RequestChromeDump(dump_type, level_of_detail);
  callback.Run(true, mojom::GlobalMemoryDumpPtr());
}
