void GpuProcessHost::RunRequestGPUInfoCallbacks(const gpu::GPUInfo& gpu_info) {
  for (auto& callback : request_gpu_info_callbacks_)
    std::move(callback).Run(gpu_info);

  request_gpu_info_callbacks_.clear();
}
