  void GenerateStartAndEndWidths(int start_tab_count, int end_tab_count,
                                 int start_mini_count,
                                 int end_mini_count) {
    tabstrip_->GetDesiredTabWidths(start_tab_count, start_mini_count,
                                   &start_unselected_width_,
                                   &start_selected_width_);
    double standard_tab_width =
        static_cast<double>(TabRendererGtk::GetStandardSize().width());

    if ((end_tab_count - start_tab_count) > 0 &&
        start_unselected_width_ < standard_tab_width) {
      double minimum_tab_width = static_cast<double>(
          TabRendererGtk::GetMinimumUnselectedSize().width());
      start_unselected_width_ -= minimum_tab_width / start_tab_count;
    }

    tabstrip_->GenerateIdealBounds();
    tabstrip_->GetDesiredTabWidths(end_tab_count, end_mini_count,
                                   &end_unselected_width_,
                                   &end_selected_width_);
  }
