void BackingStoreGtk::CairoShowRect(const gfx::Rect& rect,
                                    GdkDrawable* drawable) {
  cairo_surface_t* surface = cairo_xlib_surface_create(
      display_, pixmap_, static_cast<Visual*>(visual_),
      size().width(), size().height());
  cairo_t* cr = gdk_cairo_create(drawable);
  cairo_set_source_surface(cr, surface, 0, 0);

  cairo_rectangle(cr, rect.x(), rect.y(), rect.width(), rect.height());
  cairo_fill(cr);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}
