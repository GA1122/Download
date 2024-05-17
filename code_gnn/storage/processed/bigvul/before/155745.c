void Compositor::SetScaleAndSize(float scale,
                                 const gfx::Size& size_in_pixel,
                                 const viz::LocalSurfaceId& local_surface_id) {
  DCHECK_GT(scale, 0);
  bool device_scale_factor_changed = device_scale_factor_ != scale;
  device_scale_factor_ = scale;

  if (size_ != size_in_pixel && local_surface_id.is_valid()) {
    DCHECK_NE(local_surface_id, host_->local_surface_id_from_parent());
  }

  if (!size_in_pixel.IsEmpty()) {
    bool size_changed = size_ != size_in_pixel;
    size_ = size_in_pixel;
    host_->SetViewportSizeAndScale(size_in_pixel, scale, local_surface_id);
    root_web_layer_->SetBounds(size_in_pixel);
    if (context_factory_private_ &&
        (size_changed || disabled_swap_until_resize_)) {
      context_factory_private_->ResizeDisplay(this, size_in_pixel);
      disabled_swap_until_resize_ = false;
    }
  }
  if (device_scale_factor_changed) {
    if (is_pixel_canvas())
      host_->SetRecordingScaleFactor(scale);
    if (root_layer_)
      root_layer_->OnDeviceScaleFactorChanged(scale);
  }
}
