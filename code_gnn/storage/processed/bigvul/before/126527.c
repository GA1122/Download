  virtual double GetWidthForTab(int index) const {
    TabGtk* tab = tabstrip_->GetTabAt(index);

    if (index == to_index_)
      return animation_.CurrentValueBetween(0, target_bounds_.width());

    if (tab->mini())
      return TabGtk::GetMiniWidth();

    if (tab->IsActive()) {
      return animation_.CurrentValueBetween(start_selected_width_,
                                            end_selected_width_);
    }

    return animation_.CurrentValueBetween(start_unselected_width_,
                                          end_unselected_width_);
  }
