int TabStripGtk::GetTabHOffset(int tab_index) {
  if (tab_index < GetTabCount() && GetTabAt(tab_index - 1)->mini() &&
      !GetTabAt(tab_index)->mini()) {
    return mini_to_non_mini_gap_ + kTabHOffset;
  }
  return kTabHOffset;
}
