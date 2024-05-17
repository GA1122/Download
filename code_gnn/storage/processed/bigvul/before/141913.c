views::Label* AutofillPopupItemView::CreateLabelWithStyleAndContext(
    const base::string16& text,
    int text_context,
    int text_style) const {
  views::Label* label =
      CreateLabelWithColorReadabilityDisabled(text, text_context, text_style);
  label->SetHorizontalAlignment(gfx::ALIGN_LEFT);

  return label;
}
