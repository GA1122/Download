  virtual double GetGapWidth(int index) {
    if (to_index_ < from_index_) {
      if (index == to_index_) {
        double current_size =
            animation_.CurrentValueBetween(0, target_bounds_.width());
        if (current_size < -kTabHOffset)
          return -(current_size + kTabHOffset);
      } else if (index == from_index_ + 1) {
        return animation_.CurrentValueBetween(start_bounds_.width(), 0);
      }
    } else {
      if (index == from_index_) {
        return animation_.CurrentValueBetween(
            TabGtk::GetMiniWidth() + kTabHOffset, 0);
      }
    }
    return 0;
  }
