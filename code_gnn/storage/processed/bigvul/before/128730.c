bool TemplateURL::SupportsReplacement(
    const SearchTermsData& search_terms_data) const {
  return url_ref_->SupportsReplacement(search_terms_data);
}
