void CoordinatorImpl::RequestGlobalMemoryDumpForPid(
    base::ProcessId pid,
    const std::vector<std::string>& allocator_dump_names,
    RequestGlobalMemoryDumpForPidCallback callback) {
  if (pid == base::kNullProcessId) {
    std::move(callback).Run(false, nullptr);
    return;
  }

  auto adapter = [](RequestGlobalMemoryDumpForPidCallback callback,
                    bool success, uint64_t,
                    mojom::GlobalMemoryDumpPtr global_memory_dump) {
    std::move(callback).Run(success, std::move(global_memory_dump));
  };

  QueuedRequest::Args args(
      base::trace_event::MemoryDumpType::SUMMARY_ONLY,
      base::trace_event::MemoryDumpLevelOfDetail::BACKGROUND,
      allocator_dump_names, false  , pid,
       false);
  RequestGlobalMemoryDumpInternal(args,
                                  base::BindOnce(adapter, std::move(callback)));
}
