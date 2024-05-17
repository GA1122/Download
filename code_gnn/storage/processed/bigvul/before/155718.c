  bool IsInAxesData(vr::EVRButtonId button_id) {
    auto it = axes_data_.find(button_id);
    return it != axes_data_.end();
  }
