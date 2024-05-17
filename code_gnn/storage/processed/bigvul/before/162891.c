void ProfilingService::OnGetVmRegionsCompleteForDumpProcessesForTracing(
    bool keep_small_allocations,
    bool strip_path_from_mapped_files,
    mojom::ProfilingService::DumpProcessesForTracingCallback callback,
    bool success,
    memory_instrumentation::mojom::GlobalMemoryDumpPtr dump) {
  if (!success) {
    DLOG(ERROR) << "GetVMRegions failed";
    std::move(callback).Run(
        std::vector<profiling::mojom::SharedBufferWithSizePtr>());
    return;
  }
  connection_manager_.DumpProcessesForTracing(
      keep_small_allocations, strip_path_from_mapped_files, std::move(callback),
      std::move(dump));
}
