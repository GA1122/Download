void LayerTreeHostImpl::CreateTileManagerResources() {
  CreateResourceAndRasterBufferProvider(&raster_buffer_provider_,
                                        &resource_pool_);

  if (use_gpu_rasterization_) {
    image_decode_cache_ = base::MakeUnique<GpuImageDecodeCache>(
        compositor_frame_sink_->worker_context_provider(),
        settings_.renderer_settings.preferred_tile_format,
        settings_.gpu_decoded_image_budget_bytes);
  } else {
    image_decode_cache_ = base::MakeUnique<SoftwareImageDecodeCache>(
        settings_.renderer_settings.preferred_tile_format,
        settings_.software_decoded_image_budget_bytes);
  }

  TaskGraphRunner* task_graph_runner = task_graph_runner_;
  if (is_synchronous_single_threaded_) {
    DCHECK(!single_thread_synchronous_task_graph_runner_);
    single_thread_synchronous_task_graph_runner_.reset(
        new SynchronousTaskGraphRunner);
    task_graph_runner = single_thread_synchronous_task_graph_runner_.get();
  }

  tile_manager_.SetResources(resource_pool_.get(), image_decode_cache_.get(),
                             task_graph_runner, raster_buffer_provider_.get(),
                             is_synchronous_single_threaded_
                                 ? std::numeric_limits<size_t>::max()
                                 : settings_.scheduled_raster_task_limit,
                             use_gpu_rasterization_);
  UpdateTileManagerMemoryPolicy(ActualManagedMemoryPolicy());
}
