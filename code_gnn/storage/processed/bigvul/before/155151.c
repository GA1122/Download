void OmniboxViewViews::OnPaste() {
  const base::string16 text(GetClipboardText());

  if (text.empty() ||
      (model()->focus_state() == OMNIBOX_FOCUS_INVISIBLE &&
       std::all_of(text.begin(), text.end(), base::IsUnicodeWhitespace))) {
    return;
  }

  OnBeforePossibleChange();
  model()->OnPaste();
  state_before_change_.text.clear();
  InsertOrReplaceText(text);
  OnAfterPossibleChange(true);
}
