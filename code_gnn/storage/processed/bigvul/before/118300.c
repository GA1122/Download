void AutofillDialogViews::SuggestionView::SetLabelText(
    const base::string16& text) {
  base::string16 line_return(base::ASCIIToUTF16("\n"));
  size_t position = text.find(line_return);
  if (position == base::string16::npos) {
    label_->SetText(text);
    label_line_2_->SetVisible(false);
  } else {
    label_->SetText(text.substr(0, position));
    label_line_2_->SetText(text.substr(position + line_return.length()));
    label_line_2_->SetVisible(true);
  }
}
