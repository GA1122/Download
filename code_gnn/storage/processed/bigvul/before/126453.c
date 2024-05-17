void BrowserWindowGtk::UpdateWindowShape(int width, int height) {
  GdkRegion* mask = GetWindowShape(width, height);
  gdk_window_shape_combine_region(
      gtk_widget_get_window(GTK_WIDGET(window_)), mask, 0, 0);
  if (mask)
    gdk_region_destroy(mask);

  if (UseCustomFrame() && !IsFullscreen() && !IsMaximized()) {
    gtk_alignment_set_padding(GTK_ALIGNMENT(window_container_), 1,
        kFrameBorderThickness, kFrameBorderThickness, kFrameBorderThickness);
  } else {
    gtk_alignment_set_padding(GTK_ALIGNMENT(window_container_), 0, 0, 0, 0);
  }
}
