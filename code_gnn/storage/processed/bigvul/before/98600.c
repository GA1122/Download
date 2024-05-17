void DraggedTabGtk::SetContainerShapeMask(cairo_surface_t* surface) {
  gfx::Size size = bounds().size();
  GdkPixmap* pixmap = gdk_pixmap_new(NULL, size.width(), size.height(), 1);
  cairo_t* cairo_context = gdk_cairo_create(GDK_DRAWABLE(pixmap));

  cairo_set_source_rgba(cairo_context, 1.0f, 1.0f, 1.0f, 0.0f);

  cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
  if (!attached_)
    cairo_scale(cairo_context, kScalingFactor, kScalingFactor);
  cairo_set_source_surface(cairo_context, surface, 0, 0);
  cairo_paint(cairo_context);

  if (!attached_) {
    cairo_identity_matrix(cairo_context);
    cairo_set_source_rgba(cairo_context, 1.0f, 1.0f, 1.0f, 1.0f);
    int tab_height = static_cast<int>(kScalingFactor *
                                      renderer_->height() -
                                      kDragFrameBorderSize);
    cairo_rectangle(cairo_context,
                    0, tab_height,
                    size.width(), size.height() - tab_height);
    cairo_fill(cairo_context);
  }

  cairo_destroy(cairo_context);

  gdk_window_shape_combine_mask(container_->window, pixmap, 0, 0);
  g_object_unref(pixmap);
}
