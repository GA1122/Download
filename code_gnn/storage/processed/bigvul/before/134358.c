int TabStrip::GetStartXForNormalTabs() const {
  int mini_tab_count = GetMiniTabCount();
  if (mini_tab_count == 0)
    return 0;
  return mini_tab_count * (Tab::GetMiniWidth() + tab_h_offset()) +
      kMiniToNonMiniGap;
}
