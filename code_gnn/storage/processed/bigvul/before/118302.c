void AutofillDialogViews::SuggestionView::SetState(
    const SuggestionState& state) {
  calculated_heights_.clear();
  state_ = state;
  SetVisible(state_.visible);
  UpdateLabelText();
  SetIcon(state_.icon);
  SetTextfield(state_.extra_text, state_.extra_icon);
  PreferredSizeChanged();
}
