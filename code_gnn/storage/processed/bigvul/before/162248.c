void CommandBufferProxyImpl::CreateGpuFence(uint32_t gpu_fence_id,
                                            ClientGpuFence source) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  if (last_state_.error != gpu::error::kNoError) {
    DLOG(ERROR) << "got error=" << last_state_.error;
    return;
  }

  gfx::GpuFence* gpu_fence = gfx::GpuFence::FromClientGpuFence(source);
  gfx::GpuFenceHandle handle =
      gfx::CloneHandleForIPC(gpu_fence->GetGpuFenceHandle());
  Send(new GpuCommandBufferMsg_CreateGpuFenceFromHandle(route_id_, gpu_fence_id,
                                                        handle));
}
