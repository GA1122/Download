RenderThreadImpl::SharedCompositorWorkerContextProvider() {
  DCHECK(IsMainThread());
  if (shared_worker_context_provider_) {
    viz::ContextProvider::ScopedContextLock lock(
        shared_worker_context_provider_.get());
    if (shared_worker_context_provider_->ContextGL()
            ->GetGraphicsResetStatusKHR() == GL_NO_ERROR)
      return shared_worker_context_provider_;
  }

  scoped_refptr<gpu::GpuChannelHost> gpu_channel_host(
      EstablishGpuChannelSync());
  if (!gpu_channel_host) {
    shared_worker_context_provider_ = nullptr;
    return shared_worker_context_provider_;
  }

  int32_t stream_id = kGpuStreamIdDefault;
  gpu::SchedulingPriority stream_priority = kGpuStreamPriorityDefault;
  if (is_async_worker_context_enabled_) {
    stream_id = kGpuStreamIdWorker;
    stream_priority = kGpuStreamPriorityWorker;
  }

  bool support_locking = true;
  bool support_oop_rasterization =
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableOOPRasterization);
  shared_worker_context_provider_ = CreateOffscreenContext(
      std::move(gpu_channel_host), gpu::SharedMemoryLimits(), support_locking,
      support_oop_rasterization,
      ui::command_buffer_metrics::RENDER_WORKER_CONTEXT, stream_id,
      stream_priority);
  auto result = shared_worker_context_provider_->BindToCurrentThread();
  if (result != gpu::ContextResult::kSuccess)
    shared_worker_context_provider_ = nullptr;
  return shared_worker_context_provider_;
}
