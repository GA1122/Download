void DesktopWindowTreeHostX11::ResetWindowRegion() {
  if (custom_window_shape_) {
    XShapeCombineRegion(xdisplay_, xwindow_, ShapeBounding, 0, 0,
                        window_shape_.get(), false);
    return;
  }

  window_shape_.reset();

  if (!IsMaximized() && !IsFullscreen()) {
    SkPath window_mask;
    Widget* widget = native_widget_delegate_->AsWidget();
    if (widget->non_client_view()) {
      widget->non_client_view()->GetWindowMask(bounds_in_pixels_.size(),
                                               &window_mask);
      if (window_mask.countPoints() > 0) {
        window_shape_.reset(gfx::CreateRegionFromSkPath(window_mask));
        XShapeCombineRegion(xdisplay_, xwindow_, ShapeBounding, 0, 0,
                            window_shape_.get(), false);
        return;
      }
    }
  }

  if (ShouldUseNativeFrame()) {
    XShapeCombineMask(xdisplay_, xwindow_, ShapeBounding, 0, 0, x11::None,
                      ShapeSet);
  } else {
    XRectangle r = {0,
                    0,
                    static_cast<unsigned short>(bounds_in_pixels_.width()),
                    static_cast<unsigned short>(bounds_in_pixels_.height())};
    XShapeCombineRectangles(
        xdisplay_, xwindow_, ShapeBounding, 0, 0, &r, 1, ShapeSet, YXBanded);
  }
}
