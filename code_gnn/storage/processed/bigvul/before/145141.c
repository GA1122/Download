void GpuProcessHost::DestroyGpuMemoryBuffer(gfx::GpuMemoryBufferId id,
                                            int client_id,
                                            const gpu::SyncToken& sync_token) {
  TRACE_EVENT0("gpu", "GpuProcessHost::DestroyGpuMemoryBuffer");
  gpu_service_ptr_->DestroyGpuMemoryBuffer(id, client_id, sync_token);
}
