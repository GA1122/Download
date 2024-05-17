gfx::Rect TabStrip::GetDropBounds(int drop_index,
                                  bool drop_before,
                                  bool* is_beneath) {
  DCHECK_NE(drop_index, -1);

  Tab* tab = tab_at(std::min(drop_index, tab_count() - 1));
  int center_x = tab->x();
  const int width = tab->width();
  const int overlap = TabStyle::GetTabOverlap();
  if (drop_index < tab_count())
    center_x += drop_before ? (overlap / 2) : (width / 2);
  else
    center_x += width - (overlap / 2);

  center_x = GetMirroredXInView(center_x);

  gfx::Point drop_loc(center_x - g_drop_indicator_width / 2,
                      -g_drop_indicator_height);
  ConvertPointToScreen(this, &drop_loc);
  gfx::Rect drop_bounds(drop_loc.x(), drop_loc.y(), g_drop_indicator_width,
                        g_drop_indicator_height);

  display::Screen* screen = display::Screen::GetScreen();
  display::Display display = screen->GetDisplayMatching(drop_bounds);
  *is_beneath = !display.bounds().Contains(drop_bounds);
  if (*is_beneath)
    drop_bounds.Offset(0, drop_bounds.height() + height());

  return drop_bounds;
}
