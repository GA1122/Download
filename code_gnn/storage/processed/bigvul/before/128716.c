void TemplateURLRef::ParseHostAndSearchTermKey(
    const SearchTermsData& search_terms_data) const {
  std::string url_string(GetURL());
  base::ReplaceSubstringsAfterOffset(
      &url_string, 0, "{google:baseURL}",
      search_terms_data.GoogleBaseURLValue());
  base::ReplaceSubstringsAfterOffset(
      &url_string, 0, "{google:baseSuggestURL}",
      search_terms_data.GoogleBaseSuggestURLValue());

  GURL url(url_string);
  if (!url.is_valid())
    return;

  auto query_result = FindSearchTermsKey(url.query());
  auto ref_result = FindSearchTermsKey(url.ref());
  url::Component parameter_position;
  const bool in_query = query_result.found();
  const bool in_ref = ref_result.found();
  const bool in_path = FindSearchTermsInPath(url.path(), &parameter_position);
  if (in_query ? (in_ref || in_path) : (in_ref == in_path))
    return;   

  host_ = url.host();
  port_ = url.port();
  path_ = url.path();
  if (in_query) {
    search_term_key_ = query_result.key;
    search_term_key_location_ = url::Parsed::QUERY;
    search_term_value_prefix_ = query_result.value_prefix;
    search_term_value_suffix_ = query_result.value_suffix;
  } else if (in_ref) {
    search_term_key_ = ref_result.key;
    search_term_key_location_ = url::Parsed::REF;
    search_term_value_prefix_ = ref_result.value_prefix;
    search_term_value_suffix_ = ref_result.value_suffix;
  } else {
    DCHECK(in_path);
    DCHECK_GE(parameter_position.begin, 1);   
    search_term_key_location_ = url::Parsed::PATH;
    search_term_position_in_path_ = parameter_position.begin;
    path_.erase(parameter_position.begin, parameter_position.len);
  }
}
