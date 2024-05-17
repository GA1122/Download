TabStrip::FindClosingTabResult TabStrip::FindClosingTab(const Tab* tab) {
  DCHECK(tab->closing());
  for (auto i = tabs_closing_map_.begin(); i != tabs_closing_map_.end(); ++i) {
    auto j = std::find(i->second.begin(), i->second.end(), tab);
    if (j != i->second.end())
      return FindClosingTabResult(i, j);
  }
  NOTREACHED();
  return FindClosingTabResult(tabs_closing_map_.end(), Tabs::iterator());
}
