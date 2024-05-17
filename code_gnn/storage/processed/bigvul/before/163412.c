RenderThreadImpl::SharedMainThreadContextProvider() {
  DCHECK(IsMainThread());
  if (shared_main_thread_contexts_ &&
      shared_main_thread_contexts_->ContextGL()->GetGraphicsResetStatusKHR() ==
          GL_NO_ERROR)
    return shared_main_thread_contexts_;

  scoped_refptr<gpu::GpuChannelHost> gpu_channel_host(
      EstablishGpuChannelSync());
  if (!gpu_channel_host) {
    shared_main_thread_contexts_ = nullptr;
    return nullptr;
  }

  bool support_locking = false;
  bool support_oop_rasterization = false;
  shared_main_thread_contexts_ = CreateOffscreenContext(
      std::move(gpu_channel_host), gpu::SharedMemoryLimits(), support_locking,
      support_oop_rasterization,
      ui::command_buffer_metrics::RENDERER_MAINTHREAD_CONTEXT,
      kGpuStreamIdDefault, kGpuStreamPriorityDefault);
  auto result = shared_main_thread_contexts_->BindToCurrentThread();
  if (result != gpu::ContextResult::kSuccess)
    shared_main_thread_contexts_ = nullptr;
  return shared_main_thread_contexts_;
}
