void TabStripGtk::DropInfo::SetContainerTransparency() {
  cairo_t* cairo_context = gdk_cairo_create(gtk_widget_get_window(container));
  if (!cairo_context)
      return;


  cairo_scale(cairo_context, static_cast<double>(drop_indicator_width),
              static_cast<double>(drop_indicator_height));
  cairo_set_source_rgba(cairo_context, 1.0f, 1.0f, 1.0f, 0.0f);
  cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cairo_context);
  cairo_destroy(cairo_context);
}
