const std::string& TemplateURLRef::GetPath(
    const SearchTermsData& search_terms_data) const {
  ParseIfNecessary(search_terms_data);
  return path_;
}
