DrawResult LayerTreeHostImpl::CalculateRenderPasses(FrameData* frame) {
  DCHECK(frame->render_passes.empty());
  DCHECK(CanDraw());
  DCHECK(!active_tree_->LayerListIsEmpty());

  TrackDamageForAllSurfaces(*frame->render_surface_layer_list);

  RenderSurfaceImpl* root_surface = active_tree_->RootRenderSurface();
  bool root_surface_has_no_visible_damage =
      !root_surface->GetDamageRect().Intersects(root_surface->content_rect());
  bool root_surface_has_contributing_layers =
      !root_surface->layer_list().empty();
  bool hud_wants_to_draw_ = active_tree_->hud_layer() &&
                            active_tree_->hud_layer()->IsAnimatingHUDContents();
  bool resources_must_be_resent =
      compositor_frame_sink_->capabilities().can_force_reclaim_resources;
  bool handle_visibility_changed =
      active_tree_->GetAndResetHandleVisibilityChanged();
  if (root_surface_has_contributing_layers &&
      root_surface_has_no_visible_damage &&
      !active_tree_->property_trees()->effect_tree.HasCopyRequests() &&
      !resources_must_be_resent && !hud_wants_to_draw_ &&
      !handle_visibility_changed) {
    TRACE_EVENT0("cc",
                 "LayerTreeHostImpl::CalculateRenderPasses::EmptyDamageRect");
    frame->has_no_damage = true;
    DCHECK(!resourceless_software_draw_);
    return DRAW_SUCCESS;
  }

  TRACE_EVENT_BEGIN2(
      "cc", "LayerTreeHostImpl::CalculateRenderPasses",
      "render_surface_layer_list.size()",
      static_cast<uint64_t>(frame->render_surface_layer_list->size()),
      "RequiresHighResToDraw", RequiresHighResToDraw());

  size_t render_surface_layer_list_size =
      frame->render_surface_layer_list->size();
  for (size_t i = 0; i < render_surface_layer_list_size; ++i) {
    size_t surface_index = render_surface_layer_list_size - 1 - i;
    LayerImpl* render_surface_layer =
        (*frame->render_surface_layer_list)[surface_index];
    RenderSurfaceImpl* render_surface =
        render_surface_layer->GetRenderSurface();

    bool should_draw_into_render_pass =
        active_tree_->IsRootLayer(render_surface_layer) ||
        render_surface->contributes_to_drawn_surface() ||
        render_surface->HasCopyRequest();
    if (should_draw_into_render_pass)
      render_surface->AppendRenderPasses(frame);
  }

  for (size_t i = 0; i + 1 < frame->render_passes.size(); ++i) {
    RenderPass* pass = frame->render_passes[i].get();
    pass->damage_rect = pass->output_rect;
  }

  if (active_tree_->hud_layer()) {
    RenderPass* root_pass = frame->render_passes.back().get();
    root_pass->damage_rect = root_pass->output_rect;
  }

  const Region& unoccluded_screen_space_region =
      active_tree_->UnoccludedScreenSpaceRegion();

  DrawResult draw_result = DRAW_SUCCESS;

  int layers_drawn = 0;

  const DrawMode draw_mode = GetDrawMode();

  int num_missing_tiles = 0;
  int num_incomplete_tiles = 0;
  int64_t checkerboarded_no_recording_content_area = 0;
  int64_t checkerboarded_needs_raster_content_area = 0;
  bool have_copy_request =
      active_tree()->property_trees()->effect_tree.HasCopyRequests();
  bool have_missing_animated_tiles = false;

  LayerIterator end = LayerIterator::End(frame->render_surface_layer_list);
  for (LayerIterator it =
           LayerIterator::Begin(frame->render_surface_layer_list);
       it != end; ++it) {
    auto target_render_pass_id =
        it.target_render_surface_layer()->GetRenderSurface()->GetRenderPassId();
    RenderPass* target_render_pass =
        FindRenderPassById(frame->render_passes, target_render_pass_id);

    AppendQuadsData append_quads_data;

    RenderSurfaceImpl* render_surface = it->GetRenderSurface();
    if (it.represents_target_render_surface()) {
      if (render_surface->HasCopyRequest()) {
        active_tree()
            ->property_trees()
            ->effect_tree.TakeCopyRequestsAndTransformToSurface(
                render_surface->EffectTreeIndex(),
                &target_render_pass->copy_requests);
      }
    } else if (it.represents_contributing_render_surface() &&
               render_surface->contributes_to_drawn_surface()) {
      render_surface->AppendQuads(target_render_pass, &append_quads_data);
    } else if (it.represents_itself() && !it->visible_layer_rect().IsEmpty()) {
      bool occluded =
          it->draw_properties().occlusion_in_content_space.IsOccluded(
              it->visible_layer_rect());
      if (!occluded && it->WillDraw(draw_mode, resource_provider_.get())) {
        DCHECK_EQ(active_tree_.get(), it->layer_tree_impl());

        frame->will_draw_layers.push_back(*it);
        if (it->may_contain_video())
          frame->may_contain_video = true;

        it->AppendQuads(target_render_pass, &append_quads_data);
      }

      ++layers_drawn;
    }

    rendering_stats_instrumentation_->AddVisibleContentArea(
        append_quads_data.visible_layer_area);
    rendering_stats_instrumentation_->AddApproximatedVisibleContentArea(
        append_quads_data.approximated_visible_content_area);
    rendering_stats_instrumentation_->AddCheckerboardedVisibleContentArea(
        append_quads_data.checkerboarded_visible_content_area);
    rendering_stats_instrumentation_->AddCheckerboardedNoRecordingContentArea(
        append_quads_data.checkerboarded_no_recording_content_area);
    rendering_stats_instrumentation_->AddCheckerboardedNeedsRasterContentArea(
        append_quads_data.checkerboarded_needs_raster_content_area);

    num_missing_tiles += append_quads_data.num_missing_tiles;
    num_incomplete_tiles += append_quads_data.num_incomplete_tiles;
    checkerboarded_no_recording_content_area +=
        append_quads_data.checkerboarded_no_recording_content_area;
    checkerboarded_needs_raster_content_area +=
        append_quads_data.checkerboarded_needs_raster_content_area;
    if (append_quads_data.num_missing_tiles > 0) {
      have_missing_animated_tiles |=
          !it->was_ever_ready_since_last_transform_animation() ||
          it->screen_space_transform_is_animating();
    } else {
      it->set_was_ever_ready_since_last_transform_animation(true);
    }
  }

  if (have_missing_animated_tiles && !CommitToActiveTree())
    draw_result = DRAW_ABORTED_CHECKERBOARD_ANIMATIONS;

  if (num_incomplete_tiles || num_missing_tiles) {
    if (RequiresHighResToDraw())
      draw_result = DRAW_ABORTED_MISSING_HIGH_RES_CONTENT;
  }

  if (resourceless_software_draw_)
    draw_result = DRAW_SUCCESS;

#if DCHECK_IS_ON()
  for (const auto& render_pass : frame->render_passes) {
    for (auto* quad : render_pass->quad_list)
      DCHECK(quad->shared_quad_state);
  }
  DCHECK(frame->render_passes.back()->output_rect.origin().IsOrigin());
#endif

  if (!active_tree_->has_transparent_background()) {
    frame->render_passes.back()->has_transparent_background = false;
    AppendQuadsToFillScreen(
        active_tree_->RootScrollLayerDeviceViewportBounds(),
        frame->render_passes.back().get(), active_tree_->RootRenderSurface(),
        active_tree_->background_color(), unoccluded_screen_space_region);
  }

  RemoveRenderPasses(frame);
  DCHECK(!frame->render_passes.empty());

  if (have_copy_request) {
    active_tree()->property_trees()->effect_tree.ClearCopyRequests();

    active_tree()->set_needs_update_draw_properties();
  }

  if (active_tree_->has_ever_been_drawn()) {
    UMA_HISTOGRAM_COUNTS_100(
        "Compositing.RenderPass.AppendQuadData.NumMissingTiles",
        num_missing_tiles);
    UMA_HISTOGRAM_COUNTS_100(
        "Compositing.RenderPass.AppendQuadData.NumIncompleteTiles",
        num_incomplete_tiles);
    UMA_HISTOGRAM_COUNTS(
        "Compositing.RenderPass.AppendQuadData."
        "CheckerboardedNoRecordingContentArea",
        checkerboarded_no_recording_content_area);
    UMA_HISTOGRAM_COUNTS(
        "Compositing.RenderPass.AppendQuadData."
        "CheckerboardedNeedRasterContentArea",
        checkerboarded_needs_raster_content_area);
  }

  DCHECK(draw_mode != DRAW_MODE_RESOURCELESS_SOFTWARE ||
         frame->render_passes.size() == 1u)
      << frame->render_passes.size();

  TRACE_EVENT_END2("cc", "LayerTreeHostImpl::CalculateRenderPasses",
                   "draw_result", draw_result, "missing tiles",
                   num_missing_tiles);

  DCHECK(!have_copy_request || draw_result == DRAW_SUCCESS);

  if (have_copy_request)
    client_->SetNeedsCommitOnImplThread();

  return draw_result;
}
