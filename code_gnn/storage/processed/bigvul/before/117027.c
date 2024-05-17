void OmniboxPopupViewGtk::InvalidateLine(size_t line) {
  GdkWindow* gdk_window = gtk_widget_get_window(GTK_WIDGET(window_));
  GdkRectangle line_rect = GetRectForLine(
      line, GetWindowRect(gdk_window).width()).ToGdkRectangle();
  gdk_window_invalidate_rect(gdk_window, &line_rect, FALSE);
}
