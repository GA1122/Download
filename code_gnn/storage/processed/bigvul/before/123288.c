gfx::Rect RenderWidgetHostViewGtk::GetBoundsInRootWindow() {
  GtkWidget* toplevel = gtk_widget_get_toplevel(view_.get());
  if (!toplevel)
    return gfx::Rect();

  GdkRectangle frame_extents;
  GdkWindow* gdk_window = gtk_widget_get_window(toplevel);
  if (!gdk_window)
    return gfx::Rect();

  gdk_window_get_frame_extents(gdk_window, &frame_extents);
  return gfx::Rect(frame_extents.x, frame_extents.y,
                   frame_extents.width, frame_extents.height);
}
