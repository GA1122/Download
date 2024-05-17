views::View* AutofillPopupItemView::CreateValueLabel() {
  base::string16 text =
      popup_view_->controller()->GetElidedValueAt(line_number_);
  if (popup_view_->controller()
          ->GetSuggestionAt(line_number_)
          .is_value_secondary) {
    return CreateSecondaryLabel(text);
  }

  views::Label* text_label = CreateLabelWithStyleAndContext(
      popup_view_->controller()->GetElidedValueAt(line_number_),
      ChromeTextContext::CONTEXT_BODY_TEXT_LARGE, GetPrimaryTextStyle());

  gfx::Font::Weight font_weight;
  if (ShouldUseCustomFontWeightForPrimaryInfo(&font_weight)) {
    text_label->SetFontList(
        text_label->font_list().DeriveWithWeight(font_weight));
  }

  return text_label;
}
