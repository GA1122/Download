  explicit OnDrawCompositorFrameSink(
      scoped_refptr<ContextProvider> compositor_context_provider,
      scoped_refptr<ContextProvider> worker_context_provider,
      SharedBitmapManager* shared_bitmap_manager,
      gpu::GpuMemoryBufferManager* gpu_memory_buffer_manager,
      const RendererSettings& renderer_settings,
      base::SingleThreadTaskRunner* task_runner,
      bool synchronous_composite,
      bool force_disable_reclaim_resources,
      base::Closure invalidate_callback)
      : TestCompositorFrameSink(std::move(compositor_context_provider),
                                std::move(worker_context_provider),
                                shared_bitmap_manager,
                                gpu_memory_buffer_manager,
                                renderer_settings,
                                task_runner,
                                synchronous_composite,
                                force_disable_reclaim_resources),
        invalidate_callback_(std::move(invalidate_callback)) {}
