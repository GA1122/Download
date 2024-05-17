void GpuProcessHost::SendOutstandingReplies(
    EstablishChannelStatus failure_status) {
  DCHECK_NE(failure_status, EstablishChannelStatus::SUCCESS);
  valid_ = false;

  while (!channel_requests_.empty()) {
    auto callback = channel_requests_.front();
    channel_requests_.pop();
    std::move(callback).Run(mojo::ScopedMessagePipeHandle(), gpu::GPUInfo(),
                            gpu::GpuFeatureInfo(), failure_status);
  }

  while (!create_gpu_memory_buffer_requests_.empty()) {
    auto callback = std::move(create_gpu_memory_buffer_requests_.front());
    create_gpu_memory_buffer_requests_.pop();
    std::move(callback).Run(gfx::GpuMemoryBufferHandle(),
                            BufferCreationStatus::GPU_HOST_INVALID);
  }

  if (!send_destroying_video_surface_done_cb_.is_null())
    base::ResetAndReturn(&send_destroying_video_surface_done_cb_).Run();
}
