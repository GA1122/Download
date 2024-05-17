void OmniboxViewViews::OnPaste() {
  const base::string16 text(GetClipboardText());
  if (!text.empty()) {
    OnBeforePossibleChange();
    model()->OnPaste();
    state_before_change_.text.clear();
    InsertOrReplaceText(text);
    OnAfterPossibleChange(true);
  }
}
