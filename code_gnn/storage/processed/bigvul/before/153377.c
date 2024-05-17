void TabStrip::DoLayout() {
  last_layout_size_ = size();

  StopAnimating(false);

  SwapLayoutIfNecessary();

  if (touch_layout_)
    touch_layout_->SetWidth(GetTabAreaWidth());

  UpdateIdealBounds();

  views::ViewModelUtils::SetViewBoundsToIdealBounds(tabs_);
  SetTabVisibility();

  SchedulePaint();

  bounds_animator_.StopAnimatingView(new_tab_button_);
  new_tab_button_->SetBoundsRect(new_tab_button_bounds_);
}
