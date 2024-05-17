const std::string& TemplateURLRef::GetHost(
    const SearchTermsData& search_terms_data) const {
  ParseIfNecessary(search_terms_data);
  return host_;
}
