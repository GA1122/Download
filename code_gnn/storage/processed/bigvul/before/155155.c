void OmniboxViewViews::OnTemporaryTextMaybeChanged(
    const base::string16& display_text,
    const AutocompleteMatch& match,
    bool save_original_selection,
    bool notify_text_changed) {
  if (save_original_selection)
    saved_temporary_selection_ = GetSelectedRange();
  SetAccessibilityLabel(display_text, match);
  int caret_pos = TextAndUIDirectionMatch() ? display_text.length() : 0;
  SetWindowTextAndCaretPos(display_text, caret_pos, false, notify_text_changed);
}
