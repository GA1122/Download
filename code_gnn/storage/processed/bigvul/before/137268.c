void Textfield::OnVisibleBoundsChanged() {
  if (touch_selection_controller_)
    touch_selection_controller_->SelectionChanged();
}
