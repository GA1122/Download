bool ShouldTabShowFavicon(int capacity,
                          bool is_pinned_tab,
                          bool is_active_tab,
                          bool has_favicon,
                          TabAlertState alert_state) {
  if (!has_favicon)
    return false;
  int required_capacity = 1;
  if (ShouldTabShowCloseButton(capacity, is_pinned_tab, is_active_tab))
    ++required_capacity;
  if (ShouldTabShowAlertIndicator(capacity, is_pinned_tab, is_active_tab,
                                  has_favicon, alert_state)) {
    ++required_capacity;
  }
  return capacity >= required_capacity;
}
