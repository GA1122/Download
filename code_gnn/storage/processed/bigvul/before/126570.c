void TabStripGtk::DropInfo::SetContainerShapeMask() {
  GdkPixmap* pixmap = gdk_pixmap_new(NULL,
                                     drop_indicator_width,
                                     drop_indicator_height, 1);
  cairo_t* cairo_context = gdk_cairo_create(GDK_DRAWABLE(pixmap));

  cairo_set_source_rgba(cairo_context, 1, 1, 1, 0);

  cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
  gdk_cairo_set_source_pixbuf(cairo_context, drop_arrow->ToGdkPixbuf(), 0, 0);
  cairo_paint(cairo_context);
  cairo_destroy(cairo_context);

  GdkWindow* gdk_window = gtk_widget_get_window(container);
  gdk_window_shape_combine_mask(gdk_window, pixmap, 0, 0);
  g_object_unref(pixmap);
}
