void TabStripGtk::FinishAnimation(TabStripGtk::TabAnimation* animation,
                                  bool layout) {
  active_animation_.reset(NULL);

  for (int i = 0, count = GetTabCount(); i < count; ++i)
    GetTabAt(i)->set_animating_mini_change(false);

  if (layout)
    Layout();
}
