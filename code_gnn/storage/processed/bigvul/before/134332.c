Tab* TabStrip::FindTabForEvent(const gfx::Point& point) {
  if (touch_layout_.get()) {
    int active_tab_index = touch_layout_->active_index();
    if (active_tab_index != -1) {
      Tab* tab = FindTabForEventFrom(point, active_tab_index, -1);
      if (!tab)
        tab = FindTabForEventFrom(point, active_tab_index + 1, 1);
      return tab;
    } else if (tab_count()) {
      return FindTabForEventFrom(point, 0, 1);
    }
  } else {
    for (int i = 0; i < tab_count(); ++i) {
      if (IsPointInTab(tab_at(i), point))
        return tab_at(i);
    }
  }
  return NULL;
}
