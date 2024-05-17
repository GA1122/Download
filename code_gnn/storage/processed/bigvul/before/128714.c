bool TemplateURLRef::IsValid(const SearchTermsData& search_terms_data) const {
  ParseIfNecessary(search_terms_data);
  return valid_;
}
