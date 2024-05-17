gfx::Point RenderWidgetHostViewGtk::GetWidgetCenter() {
  if (widget_center_valid_)
    return widget_center_;

  GdkWindow* window = gtk_widget_get_window(view_.get());
  gint window_x = 0;
  gint window_y = 0;
  gdk_window_get_origin(window, &window_x, &window_y);
  gint window_w = gdk_window_get_width(window);
  gint window_h = gdk_window_get_height(window);
  widget_center_.SetPoint(window_x + window_w / 2,
                          window_y + window_h / 2);
  widget_center_valid_ = true;
  return widget_center_;
}
