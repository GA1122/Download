size_t TemplateURLRef::GetSearchTermPositionInPath(
    const SearchTermsData& search_terms_data) const {
  ParseIfNecessary(search_terms_data);
  return search_term_position_in_path_;
}
