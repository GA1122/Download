AutocompleteMatch SearchProviderTest::FindMatchWithDestination(
    const GURL& url) {
  for (ACMatches::const_iterator i = provider_->matches().begin();
       i != provider_->matches().end(); ++i) {
    if (i->destination_url == url)
      return *i;
  }
  return AutocompleteMatch(NULL, 1, false, AutocompleteMatch::HISTORY_URL);
}
