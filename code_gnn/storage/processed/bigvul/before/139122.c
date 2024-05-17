static void AppendCompositorCommandLineFlags(base::CommandLine* command_line) {
  command_line->AppendSwitchASCII(
      switches::kNumRasterThreads,
      base::IntToString(NumberOfRendererRasterThreads()));

  if (IsAsyncWorkerContextEnabled())
    command_line->AppendSwitch(switches::kEnableGpuAsyncWorkerContext);

  int msaa_sample_count = GpuRasterizationMSAASampleCount();
  if (msaa_sample_count >= 0) {
    command_line->AppendSwitchASCII(switches::kGpuRasterizationMSAASampleCount,
                                    base::IntToString(msaa_sample_count));
  }

  if (IsZeroCopyUploadEnabled())
    command_line->AppendSwitch(switches::kEnableZeroCopy);
  if (!IsPartialRasterEnabled())
    command_line->AppendSwitch(switches::kDisablePartialRaster);

  if (IsGpuMemoryBufferCompositorResourcesEnabled()) {
    command_line->AppendSwitch(
        switches::kEnableGpuMemoryBufferCompositorResources);
  }

  if (IsMainFrameBeforeActivationEnabled())
    command_line->AppendSwitch(cc::switches::kEnableMainFrameBeforeActivation);

  cc::BufferToTextureTargetMap image_targets;
  for (int usage_idx = 0; usage_idx <= static_cast<int>(gfx::BufferUsage::LAST);
       ++usage_idx) {
    gfx::BufferUsage usage = static_cast<gfx::BufferUsage>(usage_idx);
    for (int format_idx = 0;
         format_idx <= static_cast<int>(gfx::BufferFormat::LAST);
         ++format_idx) {
      gfx::BufferFormat format = static_cast<gfx::BufferFormat>(format_idx);
      uint32_t target = gpu::GetImageTextureTarget(format, usage);
      image_targets[std::make_pair(usage, format)] = target;
    }
  }
  command_line->AppendSwitchASCII(
      switches::kContentImageTextureTarget,
      cc::BufferToTextureTargetMapToString(image_targets));

  GpuDataManagerImpl* gpu_data_manager = GpuDataManagerImpl::GetInstance();
  DCHECK(gpu_data_manager);
  gpu_data_manager->AppendRendererCommandLine(command_line);

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableSlimmingPaintV2)) {
    command_line->AppendSwitch(cc::switches::kEnableLayerLists);
  }
}
