void AutocompleteResult::MergeMatchesByProvider(const ACMatches& old_matches,
                                                const ACMatches& new_matches) {
  if (new_matches.size() >= old_matches.size())
    return;

  size_t delta = old_matches.size() - new_matches.size();
  const int max_relevance = (new_matches.empty() ?
      matches_.front().relevance : new_matches[0].relevance) - 1;
  for (ACMatches::const_reverse_iterator i = old_matches.rbegin();
       i != old_matches.rend() && delta > 0; ++i) {
    if (!HasMatchByDestination(*i, new_matches)) {
      AutocompleteMatch match = *i;
      match.relevance = std::min(max_relevance, match.relevance);
      match.from_previous = true;
      AddMatch(match);
      delta--;
    }
  }
}
