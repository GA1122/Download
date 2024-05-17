bool LayerTreeHost::UpdateLayers(Layer* root_layer,
                                 ResourceUpdateQueue* queue) {
  TRACE_EVENT1("cc", "LayerTreeHost::UpdateLayers",
               "source_frame_number", source_frame_number());

  RenderSurfaceLayerList update_list;
  {
    UpdateHudLayer();

    Layer* root_scroll = FindFirstScrollableLayer(root_layer);
    Layer* page_scale_layer = page_scale_layer_;
    if (!page_scale_layer && root_scroll)
      page_scale_layer = root_scroll->parent();

    if (hud_layer_) {
      hud_layer_->PrepareForCalculateDrawProperties(
          device_viewport_size(), device_scale_factor_);
    }

    TRACE_EVENT0("cc", "LayerTreeHost::UpdateLayers::CalcDrawProps");
    bool can_render_to_separate_surface = true;
    LayerTreeHostCommon::CalcDrawPropsMainInputs inputs(
        root_layer,
        device_viewport_size(),
        gfx::Transform(),
        device_scale_factor_,
        page_scale_factor_,
        page_scale_layer,
        GetRendererCapabilities().max_texture_size,
        settings_.can_use_lcd_text,
        can_render_to_separate_surface,
        settings_.layer_transforms_should_scale_layer_contents,
        &update_list);
    LayerTreeHostCommon::CalculateDrawProperties(&inputs);

    if (total_frames_used_for_lcd_text_metrics_ <=
        kTotalFramesToUseForLCDTextMetrics) {
      LayerTreeHostCommon::CallFunctionForSubtree(
          root_layer,
          base::Bind(&LayerTreeHost::CalculateLCDTextMetricsCallback,
                     base::Unretained(this)));
      total_frames_used_for_lcd_text_metrics_++;
    }

    if (total_frames_used_for_lcd_text_metrics_ ==
        kTotalFramesToUseForLCDTextMetrics) {
      total_frames_used_for_lcd_text_metrics_++;

      UMA_HISTOGRAM_PERCENTAGE(
          "Renderer4.LCDText.PercentageOfCandidateLayers",
          lcd_text_metrics_.total_num_cc_layers_can_use_lcd_text * 100.0 /
          lcd_text_metrics_.total_num_cc_layers);
      UMA_HISTOGRAM_PERCENTAGE(
          "Renderer4.LCDText.PercentageOfAALayers",
          lcd_text_metrics_.total_num_cc_layers_will_use_lcd_text * 100.0 /
          lcd_text_metrics_.total_num_cc_layers_can_use_lcd_text);
    }
  }

  partial_texture_update_requests_ = 0;

  bool did_paint_content = false;
  bool need_more_updates = false;
  PaintLayerContents(
      update_list, queue, &did_paint_content, &need_more_updates);
  if (trigger_idle_updates_ && need_more_updates) {
    TRACE_EVENT0("cc", "LayerTreeHost::UpdateLayers::posting prepaint task");
    prepaint_callback_.Reset(base::Bind(&LayerTreeHost::TriggerPrepaint,
                                        base::Unretained(this)));
    static base::TimeDelta prepaint_delay =
        base::TimeDelta::FromMilliseconds(100);
    base::MessageLoop::current()->PostDelayedTask(
        FROM_HERE, prepaint_callback_.callback(), prepaint_delay);
  }

  return did_paint_content;
}
