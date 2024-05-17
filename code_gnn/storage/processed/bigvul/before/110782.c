AutocompleteMatch::Type AutocompleteEditModel::CurrentTextType() const {
  AutocompleteMatch match;
  GetInfoForCurrentText(&match, NULL);
  return match.type;
}
