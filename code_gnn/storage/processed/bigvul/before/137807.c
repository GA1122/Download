bool ShouldTabShowAlertIndicator(int capacity,
                                 bool is_pinned_tab,
                                 bool is_active_tab,
                                 bool has_favicon,
                                 TabAlertState alert_state) {
  if (alert_state == TabAlertState::NONE)
    return false;
  if (ShouldTabShowCloseButton(capacity, is_pinned_tab, is_active_tab))
    return capacity >= 2;
  return capacity >= 1;
}
