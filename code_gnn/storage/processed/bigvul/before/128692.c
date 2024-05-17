bool TemplateURL::FindSearchTermsInURL(
    const GURL& url,
    const SearchTermsData& search_terms_data,
    base::string16* search_terms,
    url::Parsed::ComponentType* search_term_component,
    url::Component* search_terms_position) const {
  DCHECK(search_terms);
  search_terms->clear();

  for (const TemplateURLRef& ref : url_refs_) {
    if (ref.ExtractSearchTermsFromURL(url, search_terms, search_terms_data,
        search_term_component, search_terms_position)) {
      return !search_terms->empty();
    }
  }
  return false;
}
