scoped_refptr<gpu::Buffer> CommandBufferProxyImpl::CreateTransferBuffer(
    size_t size,
    int32_t* id) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  *id = -1;

  int32_t new_id = channel_->ReserveTransferBufferId();

  std::unique_ptr<base::SharedMemory> shared_memory =
      AllocateAndMapSharedMemory(size);
  if (!shared_memory) {
    if (last_state_.error == gpu::error::kNoError)
      OnClientError(gpu::error::kOutOfBounds);
    return nullptr;
  }

  if (last_state_.error == gpu::error::kNoError) {
    base::SharedMemoryHandle handle =
        channel_->ShareToGpuProcess(shared_memory->handle());
    if (!base::SharedMemory::IsHandleValid(handle)) {
      if (last_state_.error == gpu::error::kNoError)
        OnClientError(gpu::error::kLostContext);
      return nullptr;
    }
    Send(new GpuCommandBufferMsg_RegisterTransferBuffer(route_id_, new_id,
                                                        handle, size));
  }

  *id = new_id;
  scoped_refptr<gpu::Buffer> buffer(
      gpu::MakeBufferFromSharedMemory(std::move(shared_memory), size));
  return buffer;
}
