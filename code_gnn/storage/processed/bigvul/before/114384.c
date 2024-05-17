bool GpuCommandBufferStub::IsInSameContextShareGroup(
    const GpuCommandBufferStubBase& other) const {
  return context_group_ ==
      static_cast<const GpuCommandBufferStub&>(other).context_group_;
}
