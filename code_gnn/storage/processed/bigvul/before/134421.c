void TabStrip::SetLayoutType(TabStripLayoutType layout_type,
                             bool adjust_layout) {
  adjust_layout_ = adjust_layout;
  if (layout_type == layout_type_)
    return;

  const int active_index = controller_->GetActiveIndex();
  int active_center = 0;
  if (active_index != -1) {
    active_center = ideal_bounds(active_index).x() +
        ideal_bounds(active_index).width() / 2;
  }
  layout_type_ = layout_type;
  SetResetToShrinkOnExit(false);
  SwapLayoutIfNecessary();
  if (touch_layout_.get() && active_index != -1) {
    touch_layout_->SetActiveTabLocation(
        active_center - ideal_bounds(active_index).width() / 2);
    AnimateToIdealBounds();
  }
}
