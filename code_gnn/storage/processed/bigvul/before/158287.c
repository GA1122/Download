bool RenderWidgetHostImpl::GetVisualProperties(
    VisualProperties* visual_properties,
    bool* needs_ack) {
  *visual_properties = VisualProperties();

  GetScreenInfo(&visual_properties->screen_info);

  if (delegate_) {
    visual_properties->is_fullscreen_granted =
        delegate_->IsFullscreenForCurrentTab();
    visual_properties->display_mode = delegate_->GetDisplayMode(this);
    visual_properties->zoom_level = delegate_->GetPendingPageZoomLevel();
  } else {
    visual_properties->is_fullscreen_granted = false;
    visual_properties->display_mode = blink::kWebDisplayModeBrowser;
    visual_properties->zoom_level = 0;
  }

  visual_properties->auto_resize_enabled = auto_resize_enabled_;
  visual_properties->min_size_for_auto_resize = min_size_for_auto_resize_;
  visual_properties->max_size_for_auto_resize = max_size_for_auto_resize_;

  if (view_) {
    visual_properties->new_size = view_->GetRequestedRendererSize();
    visual_properties->capture_sequence_number =
        view_->GetCaptureSequenceNumber();
    visual_properties->compositor_viewport_pixel_size =
        view_->GetCompositorViewportPixelSize();
    visual_properties->top_controls_height = view_->GetTopControlsHeight();
    visual_properties->bottom_controls_height =
        view_->GetBottomControlsHeight();
    if (IsUseZoomForDSFEnabled()) {
      float device_scale = visual_properties->screen_info.device_scale_factor;
      visual_properties->top_controls_height *= device_scale;
      visual_properties->bottom_controls_height *= device_scale;
    }
    visual_properties->browser_controls_shrink_blink_size =
        view_->DoBrowserControlsShrinkBlinkSize();
    visual_properties->visible_viewport_size = view_->GetVisibleViewportSize();
    viz::LocalSurfaceId local_surface_id = view_->GetLocalSurfaceId();
    if (local_surface_id.is_valid())
      visual_properties->local_surface_id = local_surface_id;
  }

  if (screen_orientation_type_for_testing_) {
    visual_properties->screen_info.orientation_type =
        *screen_orientation_type_for_testing_;
  }

  if (screen_orientation_angle_for_testing_) {
    visual_properties->screen_info.orientation_angle =
        *screen_orientation_angle_for_testing_;
  }

  const bool size_changed =
      !old_visual_properties_ ||
      old_visual_properties_->auto_resize_enabled !=
          visual_properties->auto_resize_enabled ||
      (old_visual_properties_->auto_resize_enabled &&
       (old_visual_properties_->min_size_for_auto_resize !=
            visual_properties->min_size_for_auto_resize ||
        old_visual_properties_->max_size_for_auto_resize !=
            visual_properties->max_size_for_auto_resize)) ||
      (!old_visual_properties_->auto_resize_enabled &&
       (old_visual_properties_->new_size != visual_properties->new_size ||
        (old_visual_properties_->compositor_viewport_pixel_size.IsEmpty() &&
         !visual_properties->compositor_viewport_pixel_size.IsEmpty())));

  viz::LocalSurfaceId old_parent_local_surface_id =
      old_visual_properties_
          ? old_visual_properties_->local_surface_id.value_or(
                viz::LocalSurfaceId())
          : viz::LocalSurfaceId();

  viz::LocalSurfaceId new_parent_local_surface_id =
      visual_properties->local_surface_id.value_or(viz::LocalSurfaceId());

  const bool parent_local_surface_id_changed =
      !old_visual_properties_ ||
      old_parent_local_surface_id.parent_sequence_number() !=
          new_parent_local_surface_id.parent_sequence_number() ||
      old_parent_local_surface_id.embed_token() !=
          new_parent_local_surface_id.embed_token();

  const bool zoom_changed =
      !old_visual_properties_ ||
      old_visual_properties_->zoom_level != visual_properties->zoom_level;

  bool dirty =
      zoom_changed || size_changed || parent_local_surface_id_changed ||
      old_visual_properties_->screen_info != visual_properties->screen_info ||
      old_visual_properties_->compositor_viewport_pixel_size !=
          visual_properties->compositor_viewport_pixel_size ||
      old_visual_properties_->is_fullscreen_granted !=
          visual_properties->is_fullscreen_granted ||
      old_visual_properties_->display_mode != visual_properties->display_mode ||
      old_visual_properties_->top_controls_height !=
          visual_properties->top_controls_height ||
      old_visual_properties_->browser_controls_shrink_blink_size !=
          visual_properties->browser_controls_shrink_blink_size ||
      old_visual_properties_->bottom_controls_height !=
          visual_properties->bottom_controls_height ||
      old_visual_properties_->visible_viewport_size !=
          visual_properties->visible_viewport_size ||
      old_visual_properties_->capture_sequence_number !=
          visual_properties->capture_sequence_number;

  *needs_ack = g_check_for_pending_visual_properties_ack &&
               !visual_properties->auto_resize_enabled &&
               !visual_properties->new_size.IsEmpty() &&
               !visual_properties->compositor_viewport_pixel_size.IsEmpty() &&
               visual_properties->local_surface_id && size_changed;

  return dirty;
}
