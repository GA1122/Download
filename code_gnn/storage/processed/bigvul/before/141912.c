views::View* PasswordPopupSuggestionView::CreateDescriptionLabel() {
  if ((origin_.empty() &&
       GetLayoutType() == PopupItemLayoutType::kTwoLinesLeadingIcon) ||
      masked_password_.empty()) {
    return nullptr;
  }

  views::Label* label = CreateSecondaryLabel(masked_password_);
  label->SetElideBehavior(gfx::TRUNCATE);
  return new ConstrainedWidthView(label, kAutofillPopupPasswordMaxWidth);
}
