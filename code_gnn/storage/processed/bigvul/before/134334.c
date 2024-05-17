views::View* TabStrip::FindTabHitByPoint(const gfx::Point& point) {
  for (int i = 0; i < tab_count(); ++i) {
    Tab* next_tab = i < (tab_count() - 1) ? tab_at(i + 1) : NULL;
    if (next_tab && next_tab->IsActive() && IsPointInTab(next_tab, point))
      return next_tab;
    if (IsPointInTab(tab_at(i), point))
      return tab_at(i);
  }

  return NULL;
}
