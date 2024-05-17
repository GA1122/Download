  void InitStartState() {
    for (int i = 0; i < tabstrip_->GetTabCount(); ++i) {
      TabGtk* current_tab = tabstrip_->GetTabAt(i);
      if (!current_tab->mini()) {
        if (current_tab->IsActive()) {
          start_selected_width_ = current_tab->width();
        } else {
          start_unselected_width_ = current_tab->width();
        }
      }
    }
  }
