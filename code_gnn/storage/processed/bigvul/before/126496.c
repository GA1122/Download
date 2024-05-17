bool GdkRectMatchesTabFaviconBounds(const GdkRectangle& gdk_rect, TabGtk* tab) {
  gfx::Rect favicon_bounds = tab->favicon_bounds();
  return gdk_rect.x == favicon_bounds.x() + tab->x() &&
      gdk_rect.y == favicon_bounds.y() + tab->y() &&
      gdk_rect.width == favicon_bounds.width() &&
      gdk_rect.height == favicon_bounds.height();
}
