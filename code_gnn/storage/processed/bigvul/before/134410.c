void TabStrip::RemoveTabFromViewModel(int index) {
  tabs_closing_map_[index].push_back(tab_at(index));
  UpdateTabsClosingMap(index + 1, -1);
  tabs_.Remove(index);
}
