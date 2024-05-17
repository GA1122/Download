void OmniboxViewViews::OnRevertTemporaryText(const base::string16& display_text,
                                             const AutocompleteMatch& match) {
  SetAccessibilityLabel(display_text, match);
  SelectRange(saved_temporary_selection_);
}
