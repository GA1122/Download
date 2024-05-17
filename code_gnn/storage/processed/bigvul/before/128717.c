void TemplateURLRef::ParseIfNecessary(
    const SearchTermsData& search_terms_data) const {
  if (!parsed_) {
    InvalidateCachedValues();
    parsed_ = true;
    parsed_url_ = ParseURL(GetURL(), &replacements_, &post_params_, &valid_);
    supports_replacements_ = false;
    if (valid_) {
      bool has_only_one_search_term = false;
      for (Replacements::const_iterator i = replacements_.begin();
           i != replacements_.end(); ++i) {
        if ((i->type == SEARCH_TERMS) ||
            (i->type == GOOGLE_UNESCAPED_SEARCH_TERMS)) {
          if (has_only_one_search_term) {
            has_only_one_search_term = false;
            break;
          }
          has_only_one_search_term = true;
          supports_replacements_ = true;
        }
      }
      if (has_only_one_search_term)
        ParseHostAndSearchTermKey(search_terms_data);
    }
  }
}
