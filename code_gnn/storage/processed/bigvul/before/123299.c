void RenderWidgetHostViewGtk::GetScreenInfo(WebScreenInfo* results) {
  GdkWindow* gdk_window = gtk_widget_get_window(view_.get());
  if (!gdk_window) {
    GdkDisplay* display = gdk_display_get_default();
    gdk_window = gdk_display_get_default_group(display);
  }
  if (!gdk_window)
    return;
  GetScreenInfoFromNativeWindow(gdk_window, results);
}
