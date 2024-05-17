void TabStripGtk::GenerateIdealBounds() {
  int tab_count = GetTabCount();
  double unselected, selected;
  GetDesiredTabWidths(tab_count, GetMiniTabCount(), &unselected, &selected);

  current_unselected_width_ = unselected;
  current_selected_width_ = selected;

  int tab_height = TabGtk::GetStandardSize().height();
  double tab_x = tab_start_x();
  for (int i = 0; i < tab_count; ++i) {
    TabGtk* tab = GetTabAt(i);
    double tab_width = unselected;
    if (tab->mini())
      tab_width = TabGtk::GetMiniWidth();
    else if (tab->IsActive())
      tab_width = selected;
    double end_of_tab = tab_x + tab_width;
    int rounded_tab_x = Round(tab_x);
    gfx::Rect state(rounded_tab_x, 0, Round(end_of_tab) - rounded_tab_x,
                    tab_height);
    tab_data_.at(i).ideal_bounds = state;
    tab_x = end_of_tab + GetTabHOffset(i + 1);
  }
}
