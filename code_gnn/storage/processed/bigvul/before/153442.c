bool TabStrip::NeedsTouchLayout() const {
  if (!stacked_layout_)
    return false;

  const int pinned_tab_count = GetPinnedTabCount();
  const int normal_count = tab_count() - pinned_tab_count;
  if (normal_count <= 1)
    return false;

  const int tab_overlap = TabStyle::GetTabOverlap();
  const int normal_width =
      (GetStackableTabWidth() - tab_overlap) * normal_count + tab_overlap;
  const int pinned_width =
      std::max(0, pinned_tab_count * TabStyle::GetPinnedWidth() - tab_overlap);
  return normal_width > (GetTabAreaWidth() - pinned_width);
}
