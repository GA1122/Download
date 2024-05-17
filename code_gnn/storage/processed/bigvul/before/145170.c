void GpuProcessHost::OnGpuMemoryBufferCreated(
    gfx::GpuMemoryBufferHandle handle) {
  TRACE_EVENT0("gpu", "GpuProcessHost::OnGpuMemoryBufferCreated");

  DCHECK(!create_gpu_memory_buffer_requests_.empty());
  auto callback = std::move(create_gpu_memory_buffer_requests_.front());
  create_gpu_memory_buffer_requests_.pop();
  std::move(callback).Run(std::move(handle), BufferCreationStatus::SUCCESS);
}
