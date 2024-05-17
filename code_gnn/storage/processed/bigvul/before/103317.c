const GPUInfo& GpuDataManager::gpu_info() const {
  base::AutoLock auto_lock(gpu_info_lock_);
  return gpu_info_;
}
