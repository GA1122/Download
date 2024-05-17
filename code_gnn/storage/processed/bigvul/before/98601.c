void DraggedTabGtk::SetContainerTransparency() {
  cairo_t* cairo_context = gdk_cairo_create(container_->window);
  if (!cairo_context)
    return;

  gfx::Size size = bounds().size();
  cairo_scale(cairo_context, static_cast<double>(size.width()),
              static_cast<double>(size.height()));
  cairo_set_source_rgba(cairo_context, 1.0f, 1.0f, 1.0f, 0.0f);
  cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cairo_context);
  cairo_destroy(cairo_context);
}
