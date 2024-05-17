bool TemplateURLRef::HasGoogleBaseURLs(
    const SearchTermsData& search_terms_data) const {
  ParseIfNecessary(search_terms_data);
  for (size_t i = 0; i < replacements_.size(); ++i) {
    if ((replacements_[i].type == GOOGLE_BASE_URL) ||
        (replacements_[i].type == GOOGLE_BASE_SUGGEST_URL))
      return true;
  }
  return false;
}
