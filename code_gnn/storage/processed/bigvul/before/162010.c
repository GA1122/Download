void BrowserChildProcessHostImpl::CreateMetricsAllocator() {
  if (!base::GlobalHistogramAllocator::Get())
    return;

  size_t memory_size;
  base::StringPiece metrics_name;
  switch (data_.process_type) {
    case PROCESS_TYPE_UTILITY:
      memory_size = 256 << 10;   
      metrics_name = "UtilityMetrics";
      break;

    case PROCESS_TYPE_ZYGOTE:
      memory_size = 64 << 10;   
      metrics_name = "ZygoteMetrics";
      break;

    case PROCESS_TYPE_SANDBOX_HELPER:
      memory_size = 64 << 10;   
      metrics_name = "SandboxHelperMetrics";
      break;

    case PROCESS_TYPE_GPU:
      memory_size = 64 << 10;   
      metrics_name = "GpuMetrics";
      break;

    case PROCESS_TYPE_PPAPI_PLUGIN:
      memory_size = 64 << 10;   
      metrics_name = "PpapiPluginMetrics";
      break;

    case PROCESS_TYPE_PPAPI_BROKER:
      memory_size = 64 << 10;   
      metrics_name = "PpapiBrokerMetrics";
      break;

    default:
      int process_type = data_.process_type;
      if (process_type >= PROCESS_TYPE_CONTENT_END)
        process_type += 1000 - PROCESS_TYPE_CONTENT_END;
      base::UmaHistogramSparse(
          "UMA.SubprocessMetricsProvider.UntrackedProcesses", process_type);
      return;
  }

  std::unique_ptr<base::SharedMemory> shm(new base::SharedMemory());
  if (!shm->CreateAndMapAnonymous(memory_size))
    return;
  metrics_allocator_.reset(new base::SharedPersistentMemoryAllocator(
      std::move(shm), static_cast<uint64_t>(data_.id), metrics_name,
       false));
}
