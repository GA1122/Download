void TabStripGtk::PaintOnlyFavicons(GdkEventExpose* event,
                                    const std::vector<int>& tabs_to_paint) {
  cairo_t* cr = gdk_cairo_create(GDK_DRAWABLE(event->window));
  for (size_t i = 0; i < tabs_to_paint.size(); ++i) {
    cairo_save(cr);
    GetTabAt(tabs_to_paint[i])->PaintFaviconArea(tabstrip_.get(), cr);
    cairo_restore(cr);
  }

  cairo_destroy(cr);
}
