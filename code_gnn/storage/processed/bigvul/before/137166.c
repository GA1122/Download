void Textfield::CreateTouchSelectionControllerAndNotifyIt() {
  if (!HasFocus())
    return;

  if (!touch_selection_controller_) {
    touch_selection_controller_.reset(
        ui::TouchEditingControllerDeprecated::Create(this));
  }
  if (touch_selection_controller_)
    touch_selection_controller_->SelectionChanged();
}
