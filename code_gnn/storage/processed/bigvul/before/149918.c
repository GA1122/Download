void LayerTreeHostImpl::CreateResourceAndRasterBufferProvider(
    std::unique_ptr<RasterBufferProvider>* raster_buffer_provider,
    std::unique_ptr<ResourcePool>* resource_pool) {
  DCHECK(GetTaskRunner());
  CHECK(resource_provider_);

  ContextProvider* compositor_context_provider =
      compositor_frame_sink_->context_provider();
  if (!compositor_context_provider) {
    *resource_pool =
        ResourcePool::Create(resource_provider_.get(), GetTaskRunner(),
                             ResourceProvider::TEXTURE_HINT_IMMUTABLE,
                             ResourcePool::kDefaultExpirationDelay);

    *raster_buffer_provider =
        BitmapRasterBufferProvider::Create(resource_provider_.get());
    return;
  }

  ContextProvider* worker_context_provider =
      compositor_frame_sink_->worker_context_provider();
  if (use_gpu_rasterization_) {
    DCHECK(worker_context_provider);

    *resource_pool = ResourcePool::Create(
        resource_provider_.get(), GetTaskRunner(),
        ResourceProvider::TEXTURE_HINT_IMMUTABLE_FRAMEBUFFER,
        ResourcePool::kDefaultExpirationDelay);

    int msaa_sample_count = use_msaa_ ? RequestedMSAASampleCount() : 0;

    *raster_buffer_provider = base::MakeUnique<GpuRasterBufferProvider>(
        compositor_context_provider, worker_context_provider,
        resource_provider_.get(), settings_.use_distance_field_text,
        msaa_sample_count, settings_.async_worker_context_enabled);
    return;
  }

  bool use_zero_copy = settings_.use_zero_copy;
  if (!use_zero_copy && !worker_context_provider) {
    LOG(ERROR)
        << "Forcing zero-copy tile initialization as worker context is missing";
    use_zero_copy = true;
  }

  if (use_zero_copy) {
    *resource_pool = ResourcePool::CreateForGpuMemoryBufferResources(
        resource_provider_.get(), GetTaskRunner(),
        gfx::BufferUsage::GPU_READ_CPU_READ_WRITE,
        ResourcePool::kDefaultExpirationDelay);

    *raster_buffer_provider = ZeroCopyRasterBufferProvider::Create(
        resource_provider_.get(),
        settings_.renderer_settings.preferred_tile_format);
    return;
  }

  *resource_pool =
      ResourcePool::Create(resource_provider_.get(), GetTaskRunner(),
                           ResourceProvider::TEXTURE_HINT_IMMUTABLE,
                           ResourcePool::kDefaultExpirationDelay);

  const int max_copy_texture_chromium_size =
      compositor_context_provider->ContextCapabilities()
          .max_copy_texture_chromium_size;

  *raster_buffer_provider = base::MakeUnique<OneCopyRasterBufferProvider>(
      GetTaskRunner(), compositor_context_provider, worker_context_provider,
      resource_provider_.get(), max_copy_texture_chromium_size,
      settings_.use_partial_raster, settings_.max_staging_buffer_usage_in_bytes,
      settings_.renderer_settings.preferred_tile_format,
      settings_.async_worker_context_enabled);
}
