AutocompleteInput::AutocompleteInput(const string16& text,
                                     const string16& desired_tld,
                                     bool prevent_inline_autocomplete,
                                     bool prefer_keyword,
                                     bool allow_exact_keyword_match,
                                     MatchesRequested matches_requested)
    : desired_tld_(desired_tld),
      prevent_inline_autocomplete_(prevent_inline_autocomplete),
      prefer_keyword_(prefer_keyword),
      allow_exact_keyword_match_(allow_exact_keyword_match),
      matches_requested_(matches_requested) {
  TrimWhitespace(text, TRIM_LEADING, &text_);

  GURL canonicalized_url;
  type_ = Parse(text_, desired_tld, &parts_, &scheme_, &canonicalized_url);

  if (type_ == INVALID)
    return;

  if (((type_ == UNKNOWN) || (type_ == REQUESTED_URL) || (type_ == URL)) &&
      canonicalized_url.is_valid() &&
      (!canonicalized_url.IsStandard() || canonicalized_url.SchemeIsFile() ||
       canonicalized_url.SchemeIsFileSystem() ||
       !canonicalized_url.host().empty()))
    canonicalized_url_ = canonicalized_url;

  RemoveForcedQueryStringIfNecessary(type_, &text_);
}
