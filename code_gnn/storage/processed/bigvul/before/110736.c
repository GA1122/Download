string16 AutocompleteInput::FormattedStringWithEquivalentMeaning(
    const GURL& url,
    const string16& formatted_url) {
  if (!net::CanStripTrailingSlash(url))
    return formatted_url;
  const string16 url_with_path(formatted_url + char16('/'));
  return (AutocompleteInput::Parse(formatted_url, string16(), NULL, NULL,
                                   NULL) ==
          AutocompleteInput::Parse(url_with_path, string16(), NULL, NULL,
                                   NULL)) ?
      formatted_url : url_with_path;
}
