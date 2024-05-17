void BackingStoreGtk::PaintToRect(const gfx::Rect& rect, GdkDrawable* target) {
  cairo_surface_t* surface = cairo_xlib_surface_create(
      display_, pixmap_, static_cast<Visual*>(visual_),
      size().width(), size().height());
  cairo_t* cr = gdk_cairo_create(target);

  cairo_translate(cr, rect.x(), rect.y());
  double x_scale = static_cast<double>(rect.width()) / size().width();
  double y_scale = static_cast<double>(rect.height()) / size().height();
  cairo_scale(cr, x_scale, y_scale);

  cairo_pattern_t* pattern = cairo_pattern_create_for_surface(surface);
  cairo_pattern_set_filter(pattern, CAIRO_FILTER_BEST);
  cairo_set_source(cr, pattern);
  cairo_pattern_destroy(pattern);

  cairo_identity_matrix(cr);

  cairo_rectangle(cr, rect.x(), rect.y(), rect.width(), rect.height());
  cairo_fill(cr);
  cairo_destroy(cr);
}
