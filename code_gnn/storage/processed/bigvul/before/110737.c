bool AutocompleteResult::HasCopiedMatches() const {
  for (ACMatches::const_iterator i = begin(); i != end(); ++i) {
    if (i->from_previous)
      return true;
  }
  return false;
}
