bool AutocompleteEditModel::OnEscapeKeyPressed() {
  if (has_temporary_text_) {
    AutocompleteMatch match;
    InfoForCurrentSelection(&match, NULL);
    if (match.destination_url != original_url_) {
      RevertTemporaryText(true);
      return true;
    }
  }

  if (!user_input_in_progress_ && view_->IsSelectAll())
    return false;

  view_->RevertAll();
  view_->SelectAll(true);
  return true;
}
