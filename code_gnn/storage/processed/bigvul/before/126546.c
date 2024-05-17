  MiniMoveAnimation(TabStripGtk* tabstrip,
                    int from_index,
                    int to_index,
                    const gfx::Rect& start_bounds)
      : TabAnimation(tabstrip, MINI_MOVE),
        tab_(tabstrip->GetTabAt(to_index)),
        start_bounds_(start_bounds),
        from_index_(from_index),
        to_index_(to_index) {
    int tab_count = tabstrip->GetTabCount();
    int start_mini_count = tabstrip->GetMiniTabCount();
    int end_mini_count = start_mini_count;
    if (tabstrip->GetTabAt(to_index)->mini())
      start_mini_count--;
    else
      start_mini_count++;
    GenerateStartAndEndWidths(tab_count, tab_count, start_mini_count,
                              end_mini_count);
    target_bounds_ = tabstrip->GetIdealBounds(to_index);
    tab_->set_animating_mini_change(true);
  }
