bool AutocompleteEditModel::CommitSuggestedText(bool skip_inline_autocomplete) {
  if (!controller_->GetInstant())
    return false;

  const string16 suggestion = view_->GetInstantSuggestion();
  if (suggestion.empty())
    return false;

  FinalizeInstantQuery(view_->GetText(), suggestion, skip_inline_autocomplete);
  return true;
}
