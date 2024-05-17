bool TemplateURL::HasSameKeywordAs(
    const TemplateURLData& other,
    const SearchTermsData& search_terms_data) const {
  return (data_.keyword() == other.keyword()) ||
      (IsGoogleSearchURLWithReplaceableKeyword(search_terms_data) &&
       TemplateURL(other).IsGoogleSearchURLWithReplaceableKeyword(
           search_terms_data));
}
