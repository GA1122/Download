gfx::Rect RenderWidgetHostViewGtk::GetViewBounds() const {
  GdkWindow* gdk_window = gtk_widget_get_window(view_.get());
  if (!gdk_window)
    return gfx::Rect(requested_size_);
  GdkRectangle window_rect;
  gdk_window_get_origin(gdk_window, &window_rect.x, &window_rect.y);
  return gfx::Rect(window_rect.x, window_rect.y,
                   requested_size_.width(), requested_size_.height());
}
