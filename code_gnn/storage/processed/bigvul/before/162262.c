void CommandBufferProxyImpl::GetGpuFence(
    uint32_t gpu_fence_id,
    base::OnceCallback<void(std::unique_ptr<gfx::GpuFence>)> callback) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  if (last_state_.error != gpu::error::kNoError) {
    DLOG(ERROR) << "got error=" << last_state_.error;
    return;
  }

  Send(new GpuCommandBufferMsg_GetGpuFenceHandle(route_id_, gpu_fence_id));
  get_gpu_fence_tasks_.emplace(gpu_fence_id, std::move(callback));
}
