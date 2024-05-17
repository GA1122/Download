  static gfx::Rect GetAlertIndicatorBounds(const Tab& tab) {
    if (!tab.alert_indicator_) {
      ADD_FAILURE();
      return gfx::Rect();
    }
    return tab.alert_indicator_->bounds();
  }
