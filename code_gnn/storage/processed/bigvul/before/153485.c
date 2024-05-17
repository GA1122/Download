void TabStrip::SwapLayoutIfNecessary() {
  bool needs_touch = NeedsTouchLayout();
  bool using_touch = touch_layout_ != nullptr;
  if (needs_touch == using_touch)
    return;

  if (needs_touch) {
    const int overlap = TabStyle::GetTabOverlap();
    touch_layout_.reset(new StackedTabStripLayout(
        gfx::Size(GetStackableTabWidth(), GetLayoutConstant(TAB_HEIGHT)),
        overlap, kStackedPadding, kMaxStackedCount, &tabs_));
    touch_layout_->SetWidth(GetTabAreaWidth());
    touch_layout_->SetXAndPinnedCount(UpdateIdealBoundsForPinnedTabs(nullptr),
                                      GetPinnedTabCount());
    touch_layout_->SetActiveIndex(controller_->GetActiveIndex());

    base::RecordAction(
        base::UserMetricsAction("StackedTab_EnteredStackedLayout"));
  } else {
    touch_layout_.reset();
  }
  PrepareForAnimation();
  UpdateIdealBounds();
  SetTabVisibility();
  AnimateToIdealBounds();
}
