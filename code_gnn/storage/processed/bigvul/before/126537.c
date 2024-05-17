bool TabStripGtk::IsCursorInTabStripZone() const {
  gfx::Point tabstrip_topleft;
  gtk_util::ConvertWidgetPointToScreen(tabstrip_.get(), &tabstrip_topleft);

  gfx::Rect bds = bounds();
  bds.set_origin(tabstrip_topleft);
  bds.set_height(bds.height() + kTabStripAnimationVSlop);

  GdkScreen* screen = gdk_screen_get_default();
  GdkDisplay* display = gdk_screen_get_display(screen);
  gint x, y;
  gdk_display_get_pointer(display, NULL, &x, &y, NULL);
  gfx::Point cursor_point(x, y);

  return bds.Contains(cursor_point);
}
