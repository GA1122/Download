LayerTreeHost::LayerTreeHost(
    LayerTreeHostClient* client,
    SharedBitmapManager* manager,
    const LayerTreeSettings& settings)
    : micro_benchmark_controller_(this),
      next_ui_resource_id_(1),
      animating_(false),
      needs_full_tree_sync_(true),
      needs_filter_context_(false),
      client_(client),
      source_frame_number_(0),
      rendering_stats_instrumentation_(RenderingStatsInstrumentation::Create()),
      output_surface_can_be_initialized_(true),
      output_surface_lost_(true),
      num_failed_recreate_attempts_(0),
      settings_(settings),
      debug_state_(settings.initial_debug_state),
      overdraw_bottom_height_(0.f),
      device_scale_factor_(1.f),
      visible_(true),
      page_scale_factor_(1.f),
      min_page_scale_factor_(1.f),
      max_page_scale_factor_(1.f),
      trigger_idle_updates_(true),
      background_color_(SK_ColorWHITE),
      has_transparent_background_(false),
      partial_texture_update_requests_(0),
      in_paint_layer_contents_(false),
      total_frames_used_for_lcd_text_metrics_(0),
      id_(s_layer_tree_host_sequence_number.GetNext() + 1),
      next_commit_forces_redraw_(false),
      shared_bitmap_manager_(manager) {
  if (settings_.accelerated_animation_enabled)
    animation_registrar_ = AnimationRegistrar::Create();
  rendering_stats_instrumentation_->set_record_rendering_stats(
      debug_state_.RecordRenderingStats());
}
