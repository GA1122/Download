void PictureLayerImpl::RecalculateRasterScales() {
  float old_raster_contents_scale = raster_contents_scale_;
  float old_raster_page_scale = raster_page_scale_;
  float old_raster_source_scale = raster_source_scale_;

  raster_device_scale_ = ideal_device_scale_;
  raster_page_scale_ = ideal_page_scale_;
  raster_source_scale_ = ideal_source_scale_;
  raster_contents_scale_ = ideal_contents_scale_;

  if (old_raster_source_scale &&
      !draw_properties().screen_space_transform_is_animating &&
      !was_screen_space_transform_animating_ &&
      old_raster_source_scale != ideal_source_scale_)
    raster_source_scale_is_fixed_ = true;

  if (raster_source_scale_is_fixed_) {
    raster_contents_scale_ /= raster_source_scale_;
    raster_source_scale_ = 1.f;
  }

  bool is_pinching = layer_tree_impl()->PinchGestureActive();
  if (is_pinching && old_raster_contents_scale) {
    bool zooming_out = old_raster_page_scale > ideal_page_scale_;
    float desired_contents_scale = old_raster_contents_scale;
    if (zooming_out) {
      while (desired_contents_scale > ideal_contents_scale_)
        desired_contents_scale /= kMaxScaleRatioDuringPinch;
    } else {
      while (desired_contents_scale < ideal_contents_scale_)
        desired_contents_scale *= kMaxScaleRatioDuringPinch;
    }
    raster_contents_scale_ = tilings_->GetSnappedContentsScale(
        desired_contents_scale, kSnapToExistingTilingRatio);
    raster_page_scale_ =
        raster_contents_scale_ / raster_device_scale_ / raster_source_scale_;
  }

  if (draw_properties().screen_space_transform_is_animating &&
      !ShouldAdjustRasterScaleDuringScaleAnimations()) {
    bool can_raster_at_maximum_scale = false;
    float maximum_scale = draw_properties().maximum_animation_contents_scale;
    if (maximum_scale) {
      gfx::Size bounds_at_maximum_scale = gfx::ToCeiledSize(
          gfx::ScaleSize(raster_source_->GetSize(), maximum_scale));
      int64 maximum_area = static_cast<int64>(bounds_at_maximum_scale.width()) *
                           static_cast<int64>(bounds_at_maximum_scale.height());
      gfx::Size viewport = layer_tree_impl()->device_viewport_size();
      int64 viewport_area = static_cast<int64>(viewport.width()) *
                            static_cast<int64>(viewport.height());
      if (maximum_area <= viewport_area)
        can_raster_at_maximum_scale = true;
    }
    if (can_raster_at_maximum_scale)
      raster_contents_scale_ = maximum_scale;
    else
      raster_contents_scale_ = 1.f * ideal_page_scale_ * ideal_device_scale_;
  }

  raster_contents_scale_ =
      std::max(raster_contents_scale_, MinimumContentsScale());

  gfx::Size raster_bounds = gfx::ToCeiledSize(
      gfx::ScaleSize(raster_source_->GetSize(), raster_contents_scale_));
  gfx::Size tile_size = CalculateTileSize(raster_bounds);
  bool tile_covers_bounds = tile_size.width() >= raster_bounds.width() &&
                            tile_size.height() >= raster_bounds.height();
  if (tile_size.IsEmpty() || tile_covers_bounds) {
    low_res_raster_contents_scale_ = raster_contents_scale_;
    return;
  }

  float low_res_factor =
      layer_tree_impl()->settings().low_res_contents_scale_factor;
  low_res_raster_contents_scale_ = std::max(
      raster_contents_scale_ * low_res_factor,
      MinimumContentsScale());
  DCHECK_LE(low_res_raster_contents_scale_, raster_contents_scale_);
  DCHECK_GE(low_res_raster_contents_scale_, MinimumContentsScale());
}
