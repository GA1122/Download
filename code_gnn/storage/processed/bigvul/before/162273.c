void CommandBufferProxyImpl::OnGetGpuFenceHandleComplete(
    uint32_t gpu_fence_id,
    const gfx::GpuFenceHandle& handle) {
  auto gpu_fence = base::MakeUnique<gfx::GpuFence>(handle);

  GetGpuFenceTaskMap::iterator it = get_gpu_fence_tasks_.find(gpu_fence_id);
  if (it == get_gpu_fence_tasks_.end()) {
    DLOG(ERROR) << "GPU process sent invalid GetGpuFenceHandle response.";
    base::AutoLock lock(last_state_lock_);
    OnGpuAsyncMessageError(gpu::error::kInvalidGpuMessage,
                           gpu::error::kLostContext);
    return;
  }
  auto callback = std::move(it->second);
  get_gpu_fence_tasks_.erase(it);
  std::move(callback).Run(std::move(gpu_fence));
}
