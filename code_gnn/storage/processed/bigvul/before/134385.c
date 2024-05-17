bool TabStrip::NeedsTouchLayout() const {
  if (layout_type_ == TAB_STRIP_LAYOUT_SHRINK)
    return false;

  int mini_tab_count = GetMiniTabCount();
  int normal_count = tab_count() - mini_tab_count;
  if (normal_count <= 1 || normal_count == mini_tab_count)
    return false;
  int x = GetStartXForNormalTabs();
  int available_width = width() - x - new_tab_button_width();
  return (Tab::GetTouchWidth() * normal_count +
          tab_h_offset() * (normal_count - 1)) > available_width;
}
