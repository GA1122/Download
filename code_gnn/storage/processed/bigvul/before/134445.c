void TabStrip::SwapLayoutIfNecessary() {
  bool needs_touch = NeedsTouchLayout();
  bool using_touch = touch_layout_.get() != NULL;
  if (needs_touch == using_touch)
    return;

  if (needs_touch) {
    gfx::Size tab_size(Tab::GetMinimumSelectedSize());
    tab_size.set_width(Tab::GetTouchWidth());
    touch_layout_.reset(new StackedTabStripLayout(
                            tab_size,
                            tab_h_offset(),
                            kStackedPadding,
                            kMaxStackedCount,
                            &tabs_));
    touch_layout_->SetWidth(width() - new_tab_button_width());
    GenerateIdealBoundsForMiniTabs(NULL);
    touch_layout_->SetXAndMiniCount(GetStartXForNormalTabs(),
                                    GetMiniTabCount());
    touch_layout_->SetActiveIndex(controller_->GetActiveIndex());
  } else {
    touch_layout_.reset();
  }
  PrepareForAnimation();
  GenerateIdealBounds();
  AnimateToIdealBounds();
}
