DrawResult LayerTreeHostImpl::PrepareToDraw(FrameData* frame) {
  TRACE_EVENT1("cc", "LayerTreeHostImpl::PrepareToDraw", "SourceFrameNumber",
               active_tree_->source_frame_number());
  if (input_handler_client_)
    input_handler_client_->ReconcileElasticOverscrollAndRootScroll();

  if (const char* client_name = GetClientNameForMetrics()) {
    size_t total_memory = 0;
    for (const PictureLayerImpl* layer : active_tree()->picture_layers())
      total_memory += layer->GetRasterSource()->GetMemoryUsage();
    if (total_memory != 0) {
      UMA_HISTOGRAM_COUNTS(
          base::StringPrintf("Compositing.%s.PictureMemoryUsageKb",
                             client_name),
          base::saturated_cast<int>(total_memory / 1024));
    }
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        base::StringPrintf("Compositing.%s.NumActiveLayers", client_name),
        base::saturated_cast<int>(active_tree_->NumLayers()), 1, 400, 20);
  }

  bool update_lcd_text = false;
  bool ok = active_tree_->UpdateDrawProperties(update_lcd_text);
  DCHECK(ok) << "UpdateDrawProperties failed during draw";

  tile_manager_.Flush();

  frame->render_surface_layer_list = &active_tree_->RenderSurfaceLayerList();
  frame->render_passes.clear();
  frame->will_draw_layers.clear();
  frame->has_no_damage = false;
  frame->may_contain_video = false;

  if (active_tree_->RootRenderSurface()) {
    gfx::Rect device_viewport_damage_rect = viewport_damage_rect_;
    viewport_damage_rect_ = gfx::Rect();

    active_tree_->RootRenderSurface()->damage_tracker()->AddDamageNextUpdate(
        device_viewport_damage_rect);
  }

  DrawResult draw_result = CalculateRenderPasses(frame);
  if (draw_result != DRAW_SUCCESS) {
    DCHECK(!resourceless_software_draw_);
    return draw_result;
  }

  return draw_result;
}
