  static double GetCurrentTabWidth(TabStripGtk* tabstrip,
                                   TabStripGtk::TabAnimation* animation,
                                   int index) {
    TabGtk* tab = tabstrip->GetTabAt(index);
    double tab_width;
    if (tab->mini()) {
      tab_width = TabGtk::GetMiniWidth();
    } else {
      double unselected, selected;
      tabstrip->GetCurrentTabWidths(&unselected, &selected);
      tab_width = tab->IsActive() ? selected : unselected;
    }

    if (animation) {
      double specified_tab_width = animation->GetWidthForTab(index);
      if (specified_tab_width != -1)
        tab_width = specified_tab_width;
    }

    return tab_width;
  }
