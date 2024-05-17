void AutocompleteResult::CopyOldMatches(const AutocompleteInput& input,
                                        const AutocompleteResult& old_matches) {
  if (old_matches.empty())
    return;

  if (empty()) {
    CopyFrom(old_matches);
    for (ACMatches::iterator i = begin(); i != end(); ++i)
      i->from_previous = true;
    return;
  }

  ProviderToMatches matches_per_provider, old_matches_per_provider;
  BuildProviderToMatches(&matches_per_provider);
  old_matches.BuildProviderToMatches(&old_matches_per_provider);
  for (ProviderToMatches::const_iterator i = old_matches_per_provider.begin();
       i != old_matches_per_provider.end(); ++i) {
    MergeMatchesByProvider(i->second, matches_per_provider[i->first]);
  }

  SortAndCull(input);
}
