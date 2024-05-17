void AutocompleteEditModel::FinalizeInstantQuery(
    const string16& input_text,
    const string16& suggest_text,
    bool skip_inline_autocomplete) {
  if (skip_inline_autocomplete) {
    const string16 final_text = input_text + suggest_text;
    view_->OnBeforePossibleChange();
    view_->SetWindowTextAndCaretPos(final_text, final_text.length(), false,
        false);
    view_->OnAfterPossibleChange();
  } else if (popup_->IsOpen()) {
    SearchProvider* search_provider =
        autocomplete_controller_->search_provider();
    if (search_provider)
      search_provider->FinalizeInstantQuery(input_text, suggest_text);
  }
}
