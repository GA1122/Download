void GpuCommandBufferStub::SetMemoryAllocation(
    const GpuMemoryAllocation& allocation) {
  allocation_ = allocation;

  SendMemoryAllocationToProxy(allocation);
}
