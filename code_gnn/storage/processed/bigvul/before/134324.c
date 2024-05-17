void TabStrip::DoLayout() {
  last_layout_size_ = size();

  StopAnimating(false);

  SwapLayoutIfNecessary();

  if (touch_layout_.get())
    touch_layout_->SetWidth(size().width() - new_tab_button_width());

  GenerateIdealBounds();

  views::ViewModelUtils::SetViewBoundsToIdealBounds(tabs_);

  SchedulePaint();

  bounds_animator_.StopAnimatingView(newtab_button_);
  newtab_button_->SetBoundsRect(newtab_button_bounds_);
}
