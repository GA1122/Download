const AutocompleteMatch& AutocompleteResult::match_at(size_t index) const {
  DCHECK(index < matches_.size());
  return matches_[index];
}
