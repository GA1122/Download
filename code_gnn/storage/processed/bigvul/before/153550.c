  static int VisibleIconCount(const Tab& tab) {
    return tab.showing_icon_ + tab.showing_alert_indicator_ +
           tab.showing_close_button_;
  }
