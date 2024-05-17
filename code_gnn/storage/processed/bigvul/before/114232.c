void CommandBufferProxyImpl::OnSetMemoryAllocation(
    const GpuMemoryAllocationForRenderer& allocation) {
  if (!memory_allocation_changed_callback_.is_null())
    memory_allocation_changed_callback_.Run(allocation);
}
