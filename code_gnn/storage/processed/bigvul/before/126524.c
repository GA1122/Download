  virtual double GetWidthForTab(int index) const {
    TabGtk* tab = tabstrip_->GetTabAt(index);

    if (index == index_) {
      if (tab->mini()) {
        return animation_.CurrentValueBetween(TabGtk::GetMiniWidth(),
                                              -kTabHOffset);
      }

      double start_width = start_unselected_width_;
      double target_width =
          std::max(abs(kTabHOffset),
                   TabGtk::GetMinimumUnselectedSize().width() + kTabHOffset);
      return animation_.CurrentValueBetween(start_width, target_width);
    }

    if (tab->mini())
      return TabGtk::GetMiniWidth();

    if (tabstrip_->available_width_for_tabs_ != -1 &&
        index_ != tabstrip_->GetTabCount() - 1) {
      return TabStripGtk::TabAnimation::GetWidthForTab(index);
    }

    if (tab->IsActive()) {
      double delta = end_selected_width_ - start_selected_width_;
      return start_selected_width_ + (delta * animation_.GetCurrentValue());
    }

    double delta = end_unselected_width_ - start_unselected_width_;
    return start_unselected_width_ + (delta * animation_.GetCurrentValue());
  }
