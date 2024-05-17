void TemplateURL::ResetKeywordIfNecessary(
    const SearchTermsData& search_terms_data,
    bool force) {
  if (IsGoogleSearchURLWithReplaceableKeyword(search_terms_data) || force) {
    DCHECK_NE(OMNIBOX_API_EXTENSION, type_);
    GURL url(GenerateSearchURL(search_terms_data));
    if (url.is_valid())
      data_.SetKeyword(GenerateKeyword(url));
  }
}
