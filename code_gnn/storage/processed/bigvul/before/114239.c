void CommandBufferProxyImpl::SetMemoryAllocationChangedCallback(
    const base::Callback<void(const GpuMemoryAllocationForRenderer&)>&
        callback) {
  memory_allocation_changed_callback_ = callback;
}
