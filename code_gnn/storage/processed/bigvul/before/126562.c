  RemoveTabAnimation(TabStripGtk* tabstrip, int index, WebContents* contents)
      : TabAnimation(tabstrip, REMOVE),
        index_(index) {
    int tab_count = tabstrip->GetTabCount();
    int start_mini_count = tabstrip->GetMiniTabCount();
    int end_mini_count = start_mini_count;
    if (index < start_mini_count)
      end_mini_count--;
    GenerateStartAndEndWidths(tab_count, tab_count - 1, start_mini_count,
                              end_mini_count);
    set_layout_on_completion(start_mini_count > 0 &&
                             (end_mini_count == 0 ||
                              (start_mini_count == end_mini_count &&
                               tab_count == start_mini_count + 1)));
  }
