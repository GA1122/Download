void TemplateURL::SetURL(const std::string& url) {
  data_.SetURL(url);
  engine_type_ = SEARCH_ENGINE_UNKNOWN;
  url_ref_->InvalidateCachedValues();
}
