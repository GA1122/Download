  GpuCommandBufferMemoryTracker(GpuChannel* channel) {
    gpu_memory_manager_tracking_group_ = new GpuMemoryTrackingGroup(
        channel->renderer_pid(),
        this,
        channel->gpu_channel_manager()->gpu_memory_manager());
  }
