void AutocompleteResult::AddMatch(const AutocompleteMatch& match) {
  DCHECK(default_match_ != end());
  DCHECK_EQ(AutocompleteMatch::SanitizeString(match.contents), match.contents);
  DCHECK_EQ(AutocompleteMatch::SanitizeString(match.description),
            match.description);
  ACMatches::iterator insertion_point =
      std::upper_bound(begin(), end(), match, &AutocompleteMatch::MoreRelevant);
  matches_difference_type default_offset = default_match_ - begin();
  if ((insertion_point - begin()) <= default_offset)
    ++default_offset;
  matches_.insert(insertion_point, match);
  default_match_ = begin() + default_offset;
}
