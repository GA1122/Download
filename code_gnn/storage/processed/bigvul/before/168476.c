void CoordinatorImpl::RequestGlobalMemoryDump(
    MemoryDumpType dump_type,
    MemoryDumpLevelOfDetail level_of_detail,
    const std::vector<std::string>& allocator_dump_names,
    RequestGlobalMemoryDumpCallback callback) {
  auto adapter = [](RequestGlobalMemoryDumpCallback callback, bool success,
                    uint64_t, mojom::GlobalMemoryDumpPtr global_memory_dump) {
    std::move(callback).Run(success, std::move(global_memory_dump));
  };

  QueuedRequest::Args args(dump_type, level_of_detail, allocator_dump_names,
                           false  , base::kNullProcessId,
                            false);
  RequestGlobalMemoryDumpInternal(args,
                                  base::BindOnce(adapter, std::move(callback)));
}
