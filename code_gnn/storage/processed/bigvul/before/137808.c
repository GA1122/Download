bool ShouldTabShowCloseButton(int capacity,
                              bool is_pinned_tab,
                              bool is_active_tab) {
  if (is_pinned_tab)
    return false;
  else if (is_active_tab)
    return true;
  else
    return capacity >= 3;
}
