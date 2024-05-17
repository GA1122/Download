void AutofillDialogViews::SuggestionView::SetTextfield(
    const base::string16& placeholder_text,
    const gfx::Image& icon) {
  textfield_->SetPlaceholderText(placeholder_text);
  textfield_->SetIcon(icon);
  textfield_->SetVisible(!placeholder_text.empty());
}
