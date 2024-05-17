views::View* AutofillPopupSuggestionView::CreateSubtextLabel() {
  if (GetLayoutType() != PopupItemLayoutType::kTwoLinesLeadingIcon)
    return AutofillPopupItemView::CreateSubtextLabel();

  base::string16 label_text =
      popup_view_->controller()->GetSuggestionAt(line_number_).additional_label;
  if (label_text.empty())
    return nullptr;

  views::Label* label = CreateLabelWithStyleAndContext(
      label_text, ChromeTextContext::CONTEXT_BODY_TEXT_SMALL,
      ChromeTextStyle::STYLE_SECONDARY);
  return label;
}
