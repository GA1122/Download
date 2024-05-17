bool TemplateURL::ShowInDefaultList(
    const SearchTermsData& search_terms_data) const {
  return data_.show_in_default_list &&
      url_ref_->SupportsReplacement(search_terms_data);
}
