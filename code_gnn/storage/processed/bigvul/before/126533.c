  InsertTabAnimation(TabStripGtk* tabstrip, int index)
      : TabAnimation(tabstrip, INSERT),
        index_(index) {
    int tab_count = tabstrip->GetTabCount();
    int end_mini_count = tabstrip->GetMiniTabCount();
    int start_mini_count = end_mini_count;
    if (index < end_mini_count)
      start_mini_count--;
    GenerateStartAndEndWidths(tab_count - 1, tab_count, start_mini_count,
                              end_mini_count);
  }
