media::GpuVideoAcceleratorFactories* RenderThreadImpl::GetGpuFactories() {
  DCHECK(IsMainThread());

  if (!gpu_factories_.empty()) {
    scoped_refptr<ui::ContextProviderCommandBuffer> shared_context_provider =
        gpu_factories_.back()->ContextProviderMainThread();
    if (shared_context_provider) {
      viz::ContextProvider::ScopedContextLock lock(
          shared_context_provider.get());
      if (lock.ContextGL()->GetGraphicsResetStatusKHR() == GL_NO_ERROR) {
        return gpu_factories_.back().get();
      } else {
        scoped_refptr<base::SingleThreadTaskRunner> media_task_runner =
            GetMediaThreadTaskRunner();
        media_task_runner->PostTask(
            FROM_HERE,
            base::BindOnce(
                base::IgnoreResult(
                    &GpuVideoAcceleratorFactoriesImpl::CheckContextLost),
                base::Unretained(gpu_factories_.back().get())));
      }
    }
  }

  const base::CommandLine* cmd_line = base::CommandLine::ForCurrentProcess();

  scoped_refptr<gpu::GpuChannelHost> gpu_channel_host =
      EstablishGpuChannelSync();
  if (!gpu_channel_host)
    return nullptr;
  gpu::SharedMemoryLimits limits = gpu::SharedMemoryLimits::ForMailboxContext();
  bool support_locking = true;
  bool support_oop_rasterization = false;
  scoped_refptr<ui::ContextProviderCommandBuffer> media_context_provider =
      CreateOffscreenContext(gpu_channel_host, limits, support_locking,
                             support_oop_rasterization,
                             ui::command_buffer_metrics::MEDIA_CONTEXT,
                             kGpuStreamIdDefault, kGpuStreamPriorityDefault);
  auto result = media_context_provider->BindToCurrentThread();
  if (result != gpu::ContextResult::kSuccess)
    return nullptr;

  scoped_refptr<base::SingleThreadTaskRunner> media_task_runner =
      GetMediaThreadTaskRunner();
  const bool enable_video_accelerator =
      !cmd_line->HasSwitch(switches::kDisableAcceleratedVideoDecode);
  const bool enable_gpu_memory_buffer_video_frames =
      !is_gpu_compositing_disabled_ &&
#if defined(OS_MACOSX) || defined(OS_LINUX)
      !cmd_line->HasSwitch(switches::kDisableGpuMemoryBufferVideoFrames);
#elif defined(OS_WIN)
      !cmd_line->HasSwitch(switches::kDisableGpuMemoryBufferVideoFrames) &&
      (cmd_line->HasSwitch(switches::kEnableGpuMemoryBufferVideoFrames) ||
       gpu_channel_host->gpu_info().supports_overlays);
#else
      cmd_line->HasSwitch(switches::kEnableGpuMemoryBufferVideoFrames);
#endif

  media::mojom::VideoEncodeAcceleratorProviderPtr vea_provider;
  gpu_->CreateVideoEncodeAcceleratorProvider(mojo::MakeRequest(&vea_provider));

  gpu_factories_.push_back(GpuVideoAcceleratorFactoriesImpl::Create(
      std::move(gpu_channel_host), base::ThreadTaskRunnerHandle::Get(),
      media_task_runner, std::move(media_context_provider),
      enable_gpu_memory_buffer_video_frames, buffer_to_texture_target_map_,
      enable_video_accelerator, vea_provider.PassInterface()));
  return gpu_factories_.back().get();
}
