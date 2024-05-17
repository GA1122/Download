AutocompleteResult::AutocompleteResult() {
  matches_.reserve(kMaxMatches);

  default_match_ = end();
}
