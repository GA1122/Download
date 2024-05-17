void RenderThreadImpl::RequestNewLayerTreeFrameSink(
    int routing_id,
    scoped_refptr<FrameSwapMessageQueue> frame_swap_message_queue,
    const GURL& url,
    const LayerTreeFrameSinkCallback& callback) {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();

  viz::ClientLayerTreeFrameSink::InitParams params;
  params.enable_surface_synchronization =
      command_line.HasSwitch(switches::kEnableSurfaceSynchronization);
  params.local_surface_id_provider =
      std::make_unique<RendererLocalSurfaceIdProvider>();

  if (command_line.HasSwitch(switches::kDisableGpuVsync) &&
      command_line.GetSwitchValueASCII(switches::kDisableGpuVsync) != "gpu") {
    params.synthetic_begin_frame_source = CreateSyntheticBeginFrameSource();
  }

#if defined(USE_AURA)
  if (IsRunningInMash()) {
    if (!RendererWindowTreeClient::Get(routing_id)) {
      callback.Run(nullptr);
      return;
    }
    bool connection_error = false;
    scoped_refptr<gpu::GpuChannelHost> channel =
        EstablishGpuChannelSync(&connection_error);
    if (connection_error)
      return;
    if (!channel) {
      callback.Run(nullptr);
      return;
    }
    RendererWindowTreeClient::Get(routing_id)
        ->RequestLayerTreeFrameSink(
            gpu_->CreateContextProvider(std::move(channel)),
            GetGpuMemoryBufferManager(), callback);
    return;
  }
#endif

  viz::mojom::CompositorFrameSinkRequest sink_request =
      mojo::MakeRequest(&params.pipes.compositor_frame_sink_info);
  viz::mojom::CompositorFrameSinkClientPtr client;
  params.pipes.client_request = mojo::MakeRequest(&client);

  if (command_line.HasSwitch(switches::kEnableVulkan)) {
    scoped_refptr<viz::VulkanContextProvider> vulkan_context_provider =
        viz::VulkanInProcessContextProvider::Create();
    if (vulkan_context_provider) {
      DCHECK(!layout_test_mode());
      frame_sink_provider_->CreateForWidget(routing_id, std::move(sink_request),
                                            std::move(client));
      callback.Run(std::make_unique<viz::ClientLayerTreeFrameSink>(
          std::move(vulkan_context_provider), &params));
      return;
    }
  }

  if (is_gpu_compositing_disabled_) {
    DCHECK(!layout_test_mode());
    frame_sink_provider_->CreateForWidget(routing_id, std::move(sink_request),
                                          std::move(client));
    params.shared_bitmap_manager = shared_bitmap_manager();
    callback.Run(std::make_unique<viz::ClientLayerTreeFrameSink>(
        nullptr, nullptr, &params));
    return;
  }

  bool connection_error = false;
  scoped_refptr<gpu::GpuChannelHost> gpu_channel_host =
      EstablishGpuChannelSync(&connection_error);
  if (connection_error)
    return;
  if (!gpu_channel_host) {
    callback.Run(nullptr);
    return;
  }

  scoped_refptr<ui::ContextProviderCommandBuffer> worker_context_provider =
      SharedCompositorWorkerContextProvider();
  if (!worker_context_provider) {
    callback.Run(nullptr);
    return;
  }

  gpu::SharedMemoryLimits limits = gpu::SharedMemoryLimits::ForMailboxContext();

  gpu::gles2::ContextCreationAttribHelper attributes;
  attributes.alpha_size = -1;
  attributes.depth_size = 0;
  attributes.stencil_size = 0;
  attributes.samples = 0;
  attributes.sample_buffers = 0;
  attributes.bind_generates_resource = false;
  attributes.lose_context_when_out_of_memory = true;

  constexpr bool automatic_flushes = false;
  constexpr bool support_locking = false;

  ui::ContextProviderCommandBuffer* share_context =
      worker_context_provider.get();
  if (IsAsyncWorkerContextEnabled())
    share_context = nullptr;

  scoped_refptr<ui::ContextProviderCommandBuffer> context_provider(
      new ui::ContextProviderCommandBuffer(
          gpu_channel_host, kGpuStreamIdDefault, kGpuStreamPriorityDefault,
          gpu::kNullSurfaceHandle, url, automatic_flushes, support_locking,
          limits, attributes, share_context,
          ui::command_buffer_metrics::RENDER_COMPOSITOR_CONTEXT));

  if (layout_test_deps_) {
    callback.Run(layout_test_deps_->CreateLayerTreeFrameSink(
        routing_id, std::move(gpu_channel_host), std::move(context_provider),
        std::move(worker_context_provider), GetGpuMemoryBufferManager(), this));
    return;
  }

#if defined(OS_ANDROID)
  if (sync_compositor_message_filter_) {
    std::unique_ptr<viz::BeginFrameSource> begin_frame_source =
        params.synthetic_begin_frame_source
            ? std::move(params.synthetic_begin_frame_source)
            : CreateExternalBeginFrameSource(routing_id);
    callback.Run(std::make_unique<SynchronousLayerTreeFrameSink>(
        std::move(context_provider), std::move(worker_context_provider),
        GetGpuMemoryBufferManager(), shared_bitmap_manager(), routing_id,
        g_next_layer_tree_frame_sink_id++, std::move(begin_frame_source),
        sync_compositor_message_filter_.get(),
        std::move(frame_swap_message_queue)));
    return;
  }
#endif
  frame_sink_provider_->CreateForWidget(routing_id, std::move(sink_request),
                                        std::move(client));
  params.gpu_memory_buffer_manager = GetGpuMemoryBufferManager();
  callback.Run(std::make_unique<viz::ClientLayerTreeFrameSink>(
      std::move(context_provider), std::move(worker_context_provider),
      &params));
}
