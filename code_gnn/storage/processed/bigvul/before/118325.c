void AutofillDialogViews::SuggestionView::UpdateLabelText() {
  int unused;
  SetLabelText(CanUseVerticallyCompactText(width(), &unused) ?
      state_.vertically_compact_text :
      state_.horizontally_compact_text);
}
