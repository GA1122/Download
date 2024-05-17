Layer::Layer()
    : needs_push_properties_(false),
      num_dependents_need_push_properties_(false),
      stacking_order_changed_(false),
      layer_id_(s_next_layer_id++),
      ignore_set_needs_commit_(false),
      parent_(NULL),
      layer_tree_host_(NULL),
      scroll_clip_layer_id_(INVALID_ID),
      should_scroll_on_main_thread_(false),
      have_wheel_event_handlers_(false),
      user_scrollable_horizontal_(true),
      user_scrollable_vertical_(true),
      is_root_for_isolated_group_(false),
      is_container_for_fixed_position_layers_(false),
      is_drawable_(false),
      hide_layer_and_subtree_(false),
      masks_to_bounds_(false),
      contents_opaque_(false),
      double_sided_(true),
      should_flatten_transform_(true),
      use_parent_backface_visibility_(false),
      draw_checkerboard_for_missing_tiles_(false),
      force_render_surface_(false),
      is_3d_sorted_(false),
      anchor_point_(0.5f, 0.5f),
      background_color_(0),
      opacity_(1.f),
      blend_mode_(SkXfermode::kSrcOver_Mode),
      anchor_point_z_(0.f),
      scroll_parent_(NULL),
      clip_parent_(NULL),
      replica_layer_(NULL),
      raster_scale_(0.f),
      client_(NULL) {
  if (layer_id_ == INT_MAX) {
    s_next_layer_id = 1;
  }

  layer_animation_controller_ = LayerAnimationController::Create(layer_id_);
  layer_animation_controller_->AddValueObserver(this);
  layer_animation_controller_->set_value_provider(this);
}
