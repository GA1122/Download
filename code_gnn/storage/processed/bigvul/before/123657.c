bool GpuCommandBufferStub::GetTotalGpuMemory(size_t* bytes) {
  *bytes = total_gpu_memory_;
  return !!total_gpu_memory_;
}
