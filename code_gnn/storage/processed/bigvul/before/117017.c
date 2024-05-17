void DrawFullImage(cairo_t* cr, GtkWidget* widget, const gfx::Image* image,
                   gint dest_x, gint dest_y) {
  gfx::CairoCachedSurface* surface = image->ToCairo();
  surface->SetSource(cr, widget, dest_x, dest_y);
  cairo_pattern_set_extend(cairo_get_source(cr), CAIRO_EXTEND_REPEAT);
  cairo_rectangle(cr, dest_x, dest_y, surface->Width(), surface->Height());
  cairo_fill(cr);
}
