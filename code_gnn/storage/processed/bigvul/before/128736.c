bool TemplateURLRef::UsesPOSTMethod(
    const SearchTermsData& search_terms_data) const {
  ParseIfNecessary(search_terms_data);
  return !post_params_.empty();
}
