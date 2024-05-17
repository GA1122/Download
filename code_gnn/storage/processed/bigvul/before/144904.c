cc::FrameSinkId RenderWidgetHostViewAura::FrameSinkIdAtPoint(
    cc::SurfaceHittestDelegate* delegate,
    const gfx::Point& point,
    gfx::Point* transformed_point) {
  DCHECK(device_scale_factor_ != 0.0f);

  gfx::Point point_in_pixels =
      gfx::ConvertPointToPixel(device_scale_factor_, point);
  cc::SurfaceId id = delegated_frame_host_
                         ? delegated_frame_host_->SurfaceIdAtPoint(
                               delegate, point_in_pixels, transformed_point)
                         : cc::SurfaceId();
  *transformed_point =
      gfx::ConvertPointToDIP(device_scale_factor_, *transformed_point);

  if (!id.is_valid())
    return GetFrameSinkId();
  return id.frame_sink_id();
}
