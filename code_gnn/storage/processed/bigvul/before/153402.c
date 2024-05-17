const Tab* TabStrip::GetLastVisibleTab() const {
  for (int i = tab_count() - 1; i >= 0; --i) {
    const Tab* tab = tab_at(i);
    if (tab->GetVisible())
      return tab;
  }
  return nullptr;
}
