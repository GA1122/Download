void TabStripGtk::ResizeLayoutTabs() {
  weak_factory_.InvalidateWeakPtrs();
  layout_factory_.InvalidateWeakPtrs();

  RemoveMessageLoopObserver();

  available_width_for_tabs_ = -1;
  int mini_tab_count = GetMiniTabCount();
  if (mini_tab_count == GetTabCount()) {
    return;
  }
  TabGtk* first_tab = GetTabAt(mini_tab_count);
  double unselected, selected;
  GetDesiredTabWidths(GetTabCount(), mini_tab_count, &unselected, &selected);
  int w = Round(first_tab->IsActive() ? selected : unselected);

  if (abs(first_tab->width() - w) > 1)
    StartResizeLayoutAnimation();
}
