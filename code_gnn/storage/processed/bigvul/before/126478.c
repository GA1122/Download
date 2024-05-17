void TabStripGtk::AnimationLayout(double unselected_width) {
  int tab_height = TabGtk::GetStandardSize().height();
  double tab_x = tab_start_x();
  for (int i = 0; i < GetTabCount(); ++i) {
    TabAnimation* animation = active_animation_.get();
    if (animation)
      tab_x += animation->GetGapWidth(i);
    double tab_width = TabAnimation::GetCurrentTabWidth(this, animation, i);
    double end_of_tab = tab_x + tab_width;
    int rounded_tab_x = Round(tab_x);
    TabGtk* tab = GetTabAt(i);
    gfx::Rect bounds(rounded_tab_x, 0, Round(end_of_tab) - rounded_tab_x,
                     tab_height);
    SetTabBounds(tab, bounds);
    tab_x = end_of_tab + GetTabHOffset(i + 1);
  }
  LayoutNewTabButton(tab_x, unselected_width);
}
