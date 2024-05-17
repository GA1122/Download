views::View* AutofillPopupSuggestionView::CreateDescriptionLabel() {
  if (GetLayoutType() == PopupItemLayoutType::kTwoLinesLeadingIcon)
    return nullptr;
  return AutofillPopupItemView::CreateDescriptionLabel();
}
