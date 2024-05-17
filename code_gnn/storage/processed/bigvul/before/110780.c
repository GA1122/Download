AutocompleteMatch AutocompleteEditModel::CurrentMatch() {
  AutocompleteMatch match;
  GetInfoForCurrentText(&match, NULL);
  return match;
}
