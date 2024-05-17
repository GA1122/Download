views::View* PasswordPopupSuggestionView::CreateSubtextLabel() {
  base::string16 text_to_use;
  if (!origin_.empty()) {
    text_to_use = origin_;
  } else if (GetLayoutType() == PopupItemLayoutType::kTwoLinesLeadingIcon) {
    text_to_use = masked_password_;
  }

  if (text_to_use.empty())
    return nullptr;

  views::Label* label = CreateSecondaryLabel(text_to_use);
  label->SetElideBehavior(gfx::ELIDE_HEAD);
  return new ConstrainedWidthView(label, kAutofillPopupUsernameMaxWidth);
}
