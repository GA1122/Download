views::Label* AutofillPopupItemView::CreateSecondaryLabel(
    const base::string16& text) const {
  return CreateLabelWithStyleAndContext(
      text, ChromeTextContext::CONTEXT_BODY_TEXT_LARGE,
      ChromeTextStyle::STYLE_SECONDARY);
}
