bool TemplateURLRef::SupportsReplacement(
    const SearchTermsData& search_terms_data) const {
  ParseIfNecessary(search_terms_data);
  return valid_ && supports_replacements_;
}
