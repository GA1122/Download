void TabStrip::RemoveTabFromViewModel(int index) {
  Tab* closing_tab = tab_at(index);
  bool closing_tab_was_active = closing_tab->IsActive();

  tabs_closing_map_[index].push_back(closing_tab);
  UpdateTabsClosingMap(index + 1, -1);
  tabs_.Remove(index);
  selected_tabs_.DecrementFrom(index);

  if (closing_tab_was_active)
    closing_tab->ActiveStateChanged();
}
