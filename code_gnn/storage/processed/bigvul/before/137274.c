void Textfield::SelectAll(bool reversed) {
  model_->SelectAll(reversed);
  if (HasSelection() && performing_user_action_)
    UpdateSelectionClipboard();
  UpdateAfterChange(false, true);
}
