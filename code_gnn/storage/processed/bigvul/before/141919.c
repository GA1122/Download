views::View* PasswordPopupSuggestionView::CreateValueLabel() {
  views::View* label = AutofillPopupSuggestionView::CreateValueLabel();
  return new ConstrainedWidthView(label, kAutofillPopupUsernameMaxWidth);
}
