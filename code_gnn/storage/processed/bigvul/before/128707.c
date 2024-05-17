bool TemplateURL::HasGoogleBaseURLs(
    const SearchTermsData& search_terms_data) const {
  for (const TemplateURLRef& ref : url_refs_) {
    if (ref.HasGoogleBaseURLs(search_terms_data))
      return true;
  }
  return suggestions_url_ref_.HasGoogleBaseURLs(search_terms_data) ||
      instant_url_ref_.HasGoogleBaseURLs(search_terms_data) ||
      image_url_ref_.HasGoogleBaseURLs(search_terms_data) ||
      new_tab_url_ref_.HasGoogleBaseURLs(search_terms_data) ||
      contextual_search_url_ref_.HasGoogleBaseURLs(search_terms_data);
}
