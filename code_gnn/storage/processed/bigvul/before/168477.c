void CoordinatorImpl::RequestGlobalMemoryDumpAndAppendToTrace(
    MemoryDumpType dump_type,
    MemoryDumpLevelOfDetail level_of_detail,
    RequestGlobalMemoryDumpAndAppendToTraceCallback callback) {
  auto adapter = [](RequestGlobalMemoryDumpAndAppendToTraceCallback callback,
                    bool success, uint64_t dump_guid,
                    mojom::GlobalMemoryDumpPtr) {
    std::move(callback).Run(success, dump_guid);
  };

  QueuedRequest::Args args(dump_type, level_of_detail, {},
                           true  , base::kNullProcessId,
                            false);
  RequestGlobalMemoryDumpInternal(args,
                                  base::BindOnce(adapter, std::move(callback)));
}
