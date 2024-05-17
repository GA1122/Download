void TabStrip::OnTouchUiChanged() {
  UpdateNewTabButtonBorder();
  new_tab_button_bounds_.set_size(new_tab_button_->GetPreferredSize());
  new_tab_button_->SetBoundsRect(new_tab_button_bounds_);
  StopAnimating(true);
  PreferredSizeChanged();
}
