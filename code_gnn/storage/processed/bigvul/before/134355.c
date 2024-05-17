gfx::Size TabStrip::GetPreferredSize() {
  int needed_width;
  if (touch_layout_.get() || adjust_layout_) {
    needed_width = Tab::GetTouchWidth() +
        (2 * kStackedPadding * kMaxStackedCount);
  } else {
    needed_width = Tab::GetMinimumSelectedSize().width();
  }
  needed_width += new_tab_button_width();
  if (immersive_style_)
    return gfx::Size(needed_width, Tab::GetImmersiveHeight());
  return gfx::Size(needed_width, Tab::GetMinimumUnselectedSize().height());
}
