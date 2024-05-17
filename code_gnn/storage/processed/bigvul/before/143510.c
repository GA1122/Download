void CompositorImpl::InitializeDisplay(
    std::unique_ptr<viz::OutputSurface> display_output_surface,
    scoped_refptr<viz::ContextProvider> context_provider) {
  DCHECK(layer_tree_frame_sink_request_pending_);

  pending_frames_ = 0;

  if (context_provider) {
    gpu_capabilities_ = context_provider->ContextCapabilities();
  }

  viz::FrameSinkManagerImpl* manager = GetFrameSinkManager();
  scoped_refptr<base::SingleThreadTaskRunner> task_runner =
      base::ThreadTaskRunnerHandle::Get();
  auto scheduler = std::make_unique<viz::DisplayScheduler>(
      root_window_->GetBeginFrameSource(), task_runner.get(),
      display_output_surface->capabilities().max_frames_pending);

  viz::RendererSettings renderer_settings;
  renderer_settings.allow_antialiasing = false;
  renderer_settings.highp_threshold_min = 2048;
  renderer_settings.auto_resize_output_surface = false;
  renderer_settings.initial_screen_size =
      display::Screen::GetScreen()
          ->GetDisplayNearestWindow(root_window_)
          .GetSizeInPixel();
  auto* gpu_memory_buffer_manager = BrowserMainLoop::GetInstance()
                                        ->gpu_channel_establish_factory()
                                        ->GetGpuMemoryBufferManager();

  const bool should_register_begin_frame_source = !display_;

  display_ = std::make_unique<viz::Display>(
      nullptr, renderer_settings, frame_sink_id_,
      std::move(display_output_surface), std::move(scheduler), task_runner);

  auto layer_tree_frame_sink = std::make_unique<viz::DirectLayerTreeFrameSink>(
      frame_sink_id_, GetHostFrameSinkManager(), manager, display_.get(),
      nullptr  , context_provider,
      nullptr  , task_runner,
      gpu_memory_buffer_manager, features::IsVizHitTestingEnabled());

  display_->SetVisible(true);
  display_->Resize(size_);
  display_->SetColorSpace(display_color_space_, display_color_space_);
  if (should_register_begin_frame_source) {
    GetFrameSinkManager()->RegisterBeginFrameSource(
        root_window_->GetBeginFrameSource(), frame_sink_id_);
  }
  host_->SetLayerTreeFrameSink(std::move(layer_tree_frame_sink));
}
