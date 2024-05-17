void TabStrip::SetStackedLayout(bool stacked_layout) {
  if (stacked_layout == stacked_layout_)
    return;

  stacked_layout_ = stacked_layout;
  SetResetToShrinkOnExit(false);
  SwapLayoutIfNecessary();

  const int active_index = controller_->GetActiveIndex();
  if (touch_layout_ && active_index != -1) {
    touch_layout_->SetActiveTabLocation(ideal_bounds(active_index).x());
    AnimateToIdealBounds();
  }

  for (int i = 0; i < tab_count(); ++i)
    tab_at(i)->Layout();
}
