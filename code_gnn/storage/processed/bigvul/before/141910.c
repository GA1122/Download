views::View* AutofillPopupItemView::CreateDescriptionLabel() {
  base::string16 text =
      popup_view_->controller()->GetElidedLabelAt(line_number_);
  return text.empty() ? nullptr : CreateSecondaryLabel(text);
}
