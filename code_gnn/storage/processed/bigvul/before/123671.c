void GpuCommandBufferStub::OnReceivedClientManagedMemoryStats(
    const GpuManagedMemoryStats& stats) {
  TRACE_EVENT0(
      "gpu",
      "GpuCommandBufferStub::OnReceivedClientManagedMemoryStats");
  GetMemoryManager()->
      SetClientManagedMemoryStats(this, stats);
}
