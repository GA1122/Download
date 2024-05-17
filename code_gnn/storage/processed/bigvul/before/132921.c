void RenderWidgetHostViewAura::CopyFromCompositingSurface(
    const gfx::Rect& src_subrect,
    const gfx::Size& dst_size,
    const base::Callback<void(bool, const SkBitmap&)>& callback,
    bool readback_config_rgb565) {
  if (!CanCopyToBitmap()) {
    callback.Run(false, SkBitmap());
    return;
  }

  const gfx::Size& dst_size_in_pixel = ConvertViewSizeToPixel(this, dst_size);
  scoped_ptr<cc::CopyOutputRequest> request =
      cc::CopyOutputRequest::CreateRequest(base::Bind(
          &RenderWidgetHostViewAura::CopyFromCompositingSurfaceHasResult,
          dst_size_in_pixel,
          readback_config_rgb565,
          callback));
  gfx::Rect src_subrect_in_pixel =
      ConvertRectToPixel(current_device_scale_factor_, src_subrect);
  request->set_area(src_subrect_in_pixel);
  RequestCopyOfOutput(request.Pass());
}
