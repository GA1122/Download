void TemplateURL::SetPrepopulateId(int id) {
  data_.prepopulate_id = id;
  const bool prepopulated = id > 0;
  for (TemplateURLRef& ref : url_refs_)
    ref.prepopulated_ = prepopulated;
  suggestions_url_ref_.prepopulated_ = prepopulated;
  instant_url_ref_.prepopulated_ = prepopulated;
  image_url_ref_.prepopulated_ = prepopulated;
  new_tab_url_ref_.prepopulated_ = prepopulated;
  contextual_search_url_ref_.prepopulated_ = prepopulated;
}
