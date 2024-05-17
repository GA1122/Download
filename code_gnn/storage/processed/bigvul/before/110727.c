void AutocompleteResult::BuildProviderToMatches(
    ProviderToMatches* provider_to_matches) const {
  for (ACMatches::const_iterator i = begin(); i != end(); ++i)
    (*provider_to_matches)[i->provider].push_back(*i);
}
