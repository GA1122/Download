TemplateURL::TemplateURL(const TemplateURLData& data, Type type)
    : data_(data),
      url_ref_(nullptr),
      suggestions_url_ref_(this, TemplateURLRef::SUGGEST),
      instant_url_ref_(this, TemplateURLRef::INSTANT),
      image_url_ref_(this, TemplateURLRef::IMAGE),
      new_tab_url_ref_(this, TemplateURLRef::NEW_TAB),
      contextual_search_url_ref_(this, TemplateURLRef::CONTEXTUAL_SEARCH),
      type_(type),
      engine_type_(SEARCH_ENGINE_UNKNOWN) {
  ResizeURLRefVector();
  SetPrepopulateId(data_.prepopulate_id);

  if (data_.search_terms_replacement_key ==
      "{google:instantExtendedEnabledKey}") {
    data_.search_terms_replacement_key = google_util::kInstantExtendedAPIParam;
  }
}
