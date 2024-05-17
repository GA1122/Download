gfx::Rect TabStrip::GetDropBounds(int drop_index,
                                  bool drop_before,
                                  bool* is_beneath) {
  DCHECK_NE(drop_index, -1);
  int center_x;
  if (drop_index < tab_count()) {
    Tab* tab = tab_at(drop_index);
    if (drop_before)
      center_x = tab->x() - (tab_h_offset() / 2);
    else
      center_x = tab->x() + (tab->width() / 2);
  } else {
    Tab* last_tab = tab_at(drop_index - 1);
    center_x = last_tab->x() + last_tab->width() + (tab_h_offset() / 2);
  }

  center_x = GetMirroredXInView(center_x);

  gfx::Point drop_loc(center_x - drop_indicator_width / 2,
                      -drop_indicator_height);
  ConvertPointToScreen(this, &drop_loc);
  gfx::Rect drop_bounds(drop_loc.x(), drop_loc.y(), drop_indicator_width,
                        drop_indicator_height);

  gfx::Screen* screen = gfx::Screen::GetScreenFor(GetWidget()->GetNativeView());
  gfx::Display display = screen->GetDisplayMatching(drop_bounds);
  *is_beneath = !display.bounds().Contains(drop_bounds);
  if (*is_beneath)
    drop_bounds.Offset(0, drop_bounds.height() + height());

  return drop_bounds;
}
