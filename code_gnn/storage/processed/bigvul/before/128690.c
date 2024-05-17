bool TemplateURL::ExtractSearchTermsFromURL(
    const GURL& url,
    const SearchTermsData& search_terms_data,
    base::string16* search_terms) const {
  return FindSearchTermsInURL(url, search_terms_data, search_terms, NULL, NULL);
}
