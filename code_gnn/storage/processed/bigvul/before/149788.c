void LayerTreeHost::FinishCommitOnImplThread(
    LayerTreeHostImpl* host_impl) {
  DCHECK(task_runner_provider_->IsImplThread());

  bool is_new_trace;
  TRACE_EVENT_IS_NEW_TRACE(&is_new_trace);
  if (is_new_trace &&
      frame_viewer_instrumentation::IsTracingLayerTreeSnapshots() &&
      root_layer()) {
    LayerTreeHostCommon::CallFunctionForEveryLayer(
        this, [](Layer* layer) { layer->SetNeedsPushProperties(); });
  }

  LayerTreeImpl* sync_tree = host_impl->sync_tree();

  if (next_commit_forces_redraw_) {
    sync_tree->ForceRedrawNextActivation();
    next_commit_forces_redraw_ = false;
  }
  if (next_commit_forces_recalculate_raster_scales_) {
    sync_tree->ForceRecalculateRasterScales();
    next_commit_forces_recalculate_raster_scales_ = false;
  }

  sync_tree->set_source_frame_number(SourceFrameNumber());

  if (needs_full_tree_sync_)
    TreeSynchronizer::SynchronizeTrees(root_layer(), sync_tree);

  PushPropertiesTo(sync_tree);

  sync_tree->PassSwapPromises(swap_promise_manager_.TakeSwapPromises());

  host_impl->SetHasGpuRasterizationTrigger(has_gpu_rasterization_trigger_);
  host_impl->SetContentIsSuitableForGpuRasterization(
      content_is_suitable_for_gpu_rasterization_);
  RecordGpuRasterizationHistogram(host_impl);

  host_impl->SetViewportSize(device_viewport_size_);
  sync_tree->SetDeviceScaleFactor(device_scale_factor_);
  host_impl->SetDebugState(debug_state_);

  sync_tree->set_ui_resource_request_queue(
      ui_resource_manager_->TakeUIResourcesRequests());

  {
    TRACE_EVENT0("cc", "LayerTreeHostInProcess::PushProperties");

    TreeSynchronizer::PushLayerProperties(this, sync_tree);

    sync_tree->property_trees()->scroll_tree.PushScrollUpdatesFromMainThread(
        property_trees(), sync_tree);

    sync_tree->UpdatePropertyTreeScrollingAndAnimationFromMainThread();

    TRACE_EVENT0("cc", "LayerTreeHostInProcess::AnimationHost::PushProperties");
    DCHECK(host_impl->mutator_host());
    mutator_host_->PushPropertiesTo(host_impl->mutator_host());
  }

  micro_benchmark_controller_.ScheduleImplBenchmarks(host_impl);
  property_trees_.ResetAllChangeTracking();
}
