  MiniTabAnimation(TabStripGtk* tabstrip, int index)
      : TabAnimation(tabstrip, MINI),
        index_(index) {
    int tab_count = tabstrip->GetTabCount();
    int start_mini_count = tabstrip->GetMiniTabCount();
    int end_mini_count = start_mini_count;
    if (tabstrip->GetTabAt(index)->mini())
      start_mini_count--;
    else
      start_mini_count++;
    tabstrip_->GetTabAt(index)->set_animating_mini_change(true);
    GenerateStartAndEndWidths(tab_count, tab_count, start_mini_count,
                              end_mini_count);
  }
