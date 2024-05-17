void CompositorImpl::CreateLayerTreeHost() {
  DCHECK(!host_);

  cc::LayerTreeSettings settings;
  settings.use_zero_copy = true;
  settings.enable_surface_synchronization = enable_surface_synchronization_;
  settings.build_hit_test_data = features::IsVizHitTestingSurfaceLayerEnabled();

  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  settings.initial_debug_state.SetRecordRenderingStats(
      command_line->HasSwitch(cc::switches::kEnableGpuBenchmarking));
  settings.initial_debug_state.show_fps_counter =
      command_line->HasSwitch(cc::switches::kUIShowFPSCounter);
  if (command_line->HasSwitch(cc::switches::kUIShowCompositedLayerBorders))
    settings.initial_debug_state.show_debug_borders.set();
  settings.single_thread_proxy_scheduler = true;
  settings.use_painted_device_scale_factor = true;

  animation_host_ = cc::AnimationHost::CreateMainInstance();

  cc::LayerTreeHost::InitParams params;
  params.client = this;
  params.task_graph_runner = &CompositorDependencies::Get().task_graph_runner;
  params.main_task_runner = base::ThreadTaskRunnerHandle::Get();
  params.settings = &settings;
  params.mutator_host = animation_host_.get();
  host_ = cc::LayerTreeHost::CreateSingleThreaded(this, std::move(params));
  DCHECK(!host_->IsVisible());
  host_->SetViewportSizeAndScale(size_, root_window_->GetDipScale(),
                                 GenerateLocalSurfaceId());

  if (needs_animate_)
    host_->SetNeedsAnimate();
}
