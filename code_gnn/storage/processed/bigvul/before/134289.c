void OmniboxViewViews::OnPaste() {
  const base::string16 text(GetClipboardText());
  if (!text.empty()) {
    model()->OnPaste();
    text_before_change_.clear();
    InsertOrReplaceText(text);
  }
}
