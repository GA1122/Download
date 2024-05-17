gfx::Rect TabStripGtk::GetDropBounds(int drop_index,
                                     bool drop_before,
                                     bool* is_beneath) {
  DCHECK_NE(drop_index, -1);
  int center_x;
  if (drop_index < GetTabCount()) {
    TabGtk* tab = GetTabAt(drop_index);
    gfx::Rect bounds = tab->GetNonMirroredBounds(tabstrip_.get());
    if (drop_before)
      center_x = bounds.x() - (kTabHOffset / 2);
    else
      center_x = bounds.x() + (bounds.width() / 2);
  } else {
    TabGtk* last_tab = GetTabAt(drop_index - 1);
    gfx::Rect bounds = last_tab->GetNonMirroredBounds(tabstrip_.get());
    center_x = bounds.x() + bounds.width() + (kTabHOffset / 2);
  }

  center_x = gtk_util::MirroredXCoordinate(tabstrip_.get(), center_x);

  gfx::Point drop_loc(center_x - drop_indicator_width / 2,
                      -drop_indicator_height);
  gtk_util::ConvertWidgetPointToScreen(tabstrip_.get(), &drop_loc);
  gfx::Rect drop_bounds(drop_loc.x(), drop_loc.y(), drop_indicator_width,
                        drop_indicator_height);

  *is_beneath = true;
  if (*is_beneath)
    drop_bounds.Offset(0, drop_bounds.height() + bounds().height());

  return drop_bounds;
}
