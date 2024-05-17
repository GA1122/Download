void TabStrip::UpdateTabsClosingMap(int index, int delta) {
  if (tabs_closing_map_.empty())
    return;

  if (delta == -1 &&
      tabs_closing_map_.find(index - 1) != tabs_closing_map_.end() &&
      tabs_closing_map_.find(index) != tabs_closing_map_.end()) {
    const std::vector<Tab*>& tabs(tabs_closing_map_[index]);
    tabs_closing_map_[index - 1].insert(
        tabs_closing_map_[index - 1].end(), tabs.begin(), tabs.end());
  }
  TabsClosingMap updated_map;
  for (TabsClosingMap::iterator i(tabs_closing_map_.begin());
       i != tabs_closing_map_.end(); ++i) {
    if (i->first > index)
      updated_map[i->first + delta] = i->second;
    else if (i->first < index)
      updated_map[i->first] = i->second;
  }
  if (delta > 0 && tabs_closing_map_.find(index) != tabs_closing_map_.end())
    updated_map[index + delta] = tabs_closing_map_[index];
  tabs_closing_map_.swap(updated_map);
}
