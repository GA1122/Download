bool TemplateURL::IsSearchURL(const GURL& url,
                              const SearchTermsData& search_terms_data) const {
  base::string16 search_terms;
  return ExtractSearchTermsFromURL(url, search_terms_data, &search_terms) &&
      !search_terms.empty();
}
