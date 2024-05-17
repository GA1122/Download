void DesktopWindowTreeHostX11::SetBoundsInPixels(
    const gfx::Rect& requested_bounds_in_pixel,
    const viz::LocalSurfaceIdAllocation& local_surface_id_allocation) {
  DCHECK(!local_surface_id_allocation.IsValid());

  gfx::Rect bounds_in_pixels(requested_bounds_in_pixel.origin(),
                             AdjustSize(requested_bounds_in_pixel.size()));
  bool origin_changed = bounds_in_pixels_.origin() != bounds_in_pixels.origin();
  bool size_changed = bounds_in_pixels_.size() != bounds_in_pixels.size();
  XWindowChanges changes = {0};
  unsigned value_mask = 0;


  if (size_changed) {
    delayed_resize_task_.Cancel();

    UpdateMinAndMaxSize();

    if (bounds_in_pixels.width() < min_size_in_pixels_.width() ||
        bounds_in_pixels.height() < min_size_in_pixels_.height() ||
        (!max_size_in_pixels_.IsEmpty() &&
         (bounds_in_pixels.width() > max_size_in_pixels_.width() ||
          bounds_in_pixels.height() > max_size_in_pixels_.height()))) {
      gfx::Size size_in_pixels = bounds_in_pixels.size();
      if (!max_size_in_pixels_.IsEmpty())
        size_in_pixels.SetToMin(max_size_in_pixels_);
      size_in_pixels.SetToMax(min_size_in_pixels_);
      bounds_in_pixels.set_size(size_in_pixels);
    }

    changes.width = bounds_in_pixels.width();
    changes.height = bounds_in_pixels.height();
    value_mask |= CWHeight | CWWidth;
  }

  if (origin_changed) {
    changes.x = bounds_in_pixels.x();
    changes.y = bounds_in_pixels.y();
    value_mask |= CWX | CWY;
  }
  if (value_mask)
    XConfigureWindow(xdisplay_, xwindow_, value_mask, &changes);

  bounds_in_pixels_ = bounds_in_pixels;

  if (origin_changed)
    native_widget_delegate_->AsWidget()->OnNativeWidgetMove();
  if (size_changed) {
    OnHostResizedInPixels(bounds_in_pixels.size(), local_surface_id_allocation);
    ResetWindowRegion();
  }
}
