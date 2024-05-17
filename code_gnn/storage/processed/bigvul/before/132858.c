bool PictureLayerImpl::ShouldAdjustRasterScale() const {
  if (was_screen_space_transform_animating_ !=
      draw_properties().screen_space_transform_is_animating)
    return true;

  if (draw_properties().screen_space_transform_is_animating &&
      raster_contents_scale_ != ideal_contents_scale_ &&
      ShouldAdjustRasterScaleDuringScaleAnimations())
    return true;

  bool is_pinching = layer_tree_impl()->PinchGestureActive();
  if (is_pinching && raster_page_scale_) {
    float ratio = ideal_page_scale_ / raster_page_scale_;
    if (raster_page_scale_ > ideal_page_scale_ ||
        ratio > kMaxScaleRatioDuringPinch)
      return true;
  }

  if (!is_pinching) {
    if (raster_page_scale_ != ideal_page_scale_)
      return true;
  }

  if (raster_device_scale_ != ideal_device_scale_)
    return true;

  if (!draw_properties().screen_space_transform_is_animating &&
      !raster_source_scale_is_fixed_ &&
      raster_source_scale_ != ideal_source_scale_)
    return true;

  return false;
}
