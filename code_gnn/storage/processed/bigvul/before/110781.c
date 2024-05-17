bool AutocompleteEditModel::CurrentTextIsURL() const {
  if (!user_input_in_progress_)
    return true;

  AutocompleteMatch match;
  GetInfoForCurrentText(&match, NULL);
  return match.transition == content::PAGE_TRANSITION_TYPED;
}
