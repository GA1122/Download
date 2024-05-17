void TemplateURL::ResizeURLRefVector() {
  const size_t new_size = data_.alternate_urls.size() + 1;
  if (url_refs_.size() == new_size)
    return;

  url_refs_.clear();
  url_refs_.reserve(new_size);
  for (size_t i = 0; i != data_.alternate_urls.size(); ++i)
    url_refs_.emplace_back(this, i);
  url_refs_.emplace_back(this, TemplateURLRef::SEARCH);
  url_ref_ = &url_refs_.back();
}
