Tab* TabStrip::FindTabHitByPoint(const gfx::Point& point) {
  for (int i = 0; i < tab_count(); ++i) {
    Tab* tab = tab_at(i);
    if (!IsPointInTab(tab, point))
      continue;

    Tab* next_tab = i < (tab_count() - 1) ? tab_at(i + 1) : nullptr;
    if (next_tab &&
        (next_tab->IsActive() ||
         (next_tab->IsSelected() && !tab->IsSelected())) &&
        IsPointInTab(next_tab, point))
      return next_tab;

    return tab;
  }

  for (const auto& index_and_tabs : tabs_closing_map_) {
    for (Tab* tab : index_and_tabs.second) {
      if (IsPointInTab(tab, point))
        return tab;
    }
  }

  return nullptr;
}
