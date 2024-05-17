  MoveTabAnimation(TabStripGtk* tabstrip, int tab_a_index, int tab_b_index)
      : TabAnimation(tabstrip, MOVE),
        start_tab_a_bounds_(tabstrip_->GetIdealBounds(tab_b_index)),
        start_tab_b_bounds_(tabstrip_->GetIdealBounds(tab_a_index)) {
    tab_a_ = tabstrip_->GetTabAt(tab_a_index);
    tab_b_ = tabstrip_->GetTabAt(tab_b_index);

    set_layout_on_completion(true);
  }
