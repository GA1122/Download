void TabStripGtk::Layout() {
  StopAnimation();

  GenerateIdealBounds();
  int tab_count = GetTabCount();
  int tab_right = 0;
  for (int i = 0; i < tab_count; ++i) {
    const gfx::Rect& bounds = tab_data_.at(i).ideal_bounds;
    TabGtk* tab = GetTabAt(i);
    tab->set_animating_mini_change(false);
    tab->set_vertical_offset(tab_vertical_offset_);
    SetTabBounds(tab, bounds);
    tab_right = bounds.right();
    tab_right += GetTabHOffset(i + 1);
  }

  LayoutNewTabButton(static_cast<double>(tab_right), current_unselected_width_);
}
