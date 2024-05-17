void GpuProcessHost::CreateGpuMemoryBuffer(
    gfx::GpuMemoryBufferId id,
    const gfx::Size& size,
    gfx::BufferFormat format,
    gfx::BufferUsage usage,
    int client_id,
    gpu::SurfaceHandle surface_handle,
    CreateGpuMemoryBufferCallback callback) {
  TRACE_EVENT0("gpu", "GpuProcessHost::CreateGpuMemoryBuffer");

  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  create_gpu_memory_buffer_requests_.push(std::move(callback));
  gpu_service_ptr_->CreateGpuMemoryBuffer(
      id, size, format, usage, client_id, surface_handle,
      base::BindOnce(&GpuProcessHost::OnGpuMemoryBufferCreated,
                     weak_ptr_factory_.GetWeakPtr()));
}
