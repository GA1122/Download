SearchEngineType TemplateURL::GetEngineType(
    const SearchTermsData& search_terms_data) const {
  if (engine_type_ == SEARCH_ENGINE_UNKNOWN) {
    const GURL url = GenerateSearchURL(search_terms_data);
    engine_type_ = url.is_valid() ?
        TemplateURLPrepopulateData::GetEngineType(url) : SEARCH_ENGINE_OTHER;
    DCHECK_NE(SEARCH_ENGINE_UNKNOWN, engine_type_);
  }
  return engine_type_;
}
