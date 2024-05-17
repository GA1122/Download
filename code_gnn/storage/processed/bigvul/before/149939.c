bool LayerTreeHostImpl::DrawLayers(FrameData* frame) {
  DCHECK(CanDraw());
  DCHECK_EQ(frame->has_no_damage, frame->render_passes.empty());

  TRACE_EVENT0("cc,benchmark", "LayerTreeHostImpl::DrawLayers");

  ResetRequiresHighResToDraw();

  if (frame->has_no_damage) {
    DCHECK(!resourceless_software_draw_);

    TRACE_EVENT_INSTANT0("cc", "EarlyOut_NoDamage", TRACE_EVENT_SCOPE_THREAD);
    active_tree()->BreakSwapPromises(SwapPromise::SWAP_FAILS);
    return false;
  }

  fps_counter_->SaveTimeStamp(CurrentBeginFrameArgs().frame_time,
                              !compositor_frame_sink_->context_provider());
  rendering_stats_instrumentation_->IncrementFrameCount(1);

  memory_history_->SaveEntry(tile_manager_.memory_stats_from_last_assign());

  if (debug_state_.ShowHudRects()) {
    debug_rect_history_->SaveDebugRectsForCurrentFrame(
        active_tree(), active_tree_->hud_layer(),
        *frame->render_surface_layer_list, debug_state_);
  }

  bool is_new_trace;
  TRACE_EVENT_IS_NEW_TRACE(&is_new_trace);
  if (is_new_trace) {
    if (pending_tree_) {
      LayerTreeHostCommon::CallFunctionForEveryLayer(
          pending_tree(), [](LayerImpl* layer) { layer->DidBeginTracing(); });
    }
    LayerTreeHostCommon::CallFunctionForEveryLayer(
        active_tree(), [](LayerImpl* layer) { layer->DidBeginTracing(); });
  }

  {
    TRACE_EVENT0("cc", "DrawLayers.FrameViewerTracing");
    TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID(
        frame_viewer_instrumentation::kCategoryLayerTree,
        "cc::LayerTreeHostImpl", id_, AsValueWithFrame(frame));
  }

  const DrawMode draw_mode = GetDrawMode();

  if (active_tree_->hud_layer()) {
    TRACE_EVENT0("cc", "DrawLayers.UpdateHudTexture");
    active_tree_->hud_layer()->UpdateHudTexture(draw_mode,
                                                resource_provider_.get());
  }

  CompositorFrameMetadata metadata = MakeCompositorFrameMetadata();
  metadata.may_contain_video = frame->may_contain_video;
  active_tree()->FinishSwapPromises(&metadata);
  for (auto& latency : metadata.latency_info) {
    TRACE_EVENT_WITH_FLOW1("input,benchmark", "LatencyInfo.Flow",
                           TRACE_ID_DONT_MANGLE(latency.trace_id()),
                           TRACE_EVENT_FLAG_FLOW_IN | TRACE_EVENT_FLAG_FLOW_OUT,
                           "step", "SwapBuffers");
    if (!latency.FindLatency(ui::INPUT_EVENT_LATENCY_RENDERER_SWAP_COMPONENT, 0,
                             nullptr)) {
      latency.AddLatencyNumber(ui::INPUT_EVENT_LATENCY_RENDERER_SWAP_COMPONENT,
                               0, 0);
    }
  }

  ResourceProvider::ResourceIdArray resources;
  for (const auto& render_pass : frame->render_passes) {
    for (auto* quad : render_pass->quad_list) {
      for (ResourceId resource_id : quad->resources)
        resources.push_back(resource_id);
    }
  }

  CompositorFrame compositor_frame;
  compositor_frame.metadata = std::move(metadata);
  resource_provider_->PrepareSendToParent(resources,
                                          &compositor_frame.resource_list);
  compositor_frame.render_pass_list = std::move(frame->render_passes);
  compositor_frame_sink_->SubmitCompositorFrame(std::move(compositor_frame));

  active_tree()->ClearSwapPromises();

  for (size_t i = 0; i < frame->render_surface_layer_list->size(); i++) {
    auto* surface = (*frame->render_surface_layer_list)[i]->GetRenderSurface();
    surface->damage_tracker()->DidDrawDamagedArea();
  }
  active_tree_->ResetAllChangeTracking();

  active_tree_->set_has_ever_been_drawn(true);
  devtools_instrumentation::DidDrawFrame(id_);
  benchmark_instrumentation::IssueImplThreadRenderingStatsEvent(
      rendering_stats_instrumentation_->impl_thread_rendering_stats());
  rendering_stats_instrumentation_->AccumulateAndClearImplThreadStats();
  return true;
}
