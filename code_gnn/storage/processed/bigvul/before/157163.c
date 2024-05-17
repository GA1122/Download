void UrlIndex::WaitToLoad(UrlData* url_data) {
  if (loading_.find(url_data) != loading_.end()) {
    url_data->LoadNow();
    return;
  }
  if (loading_.size() < GetMaxParallelPreload()) {
    loading_.insert(url_data);
    url_data->LoadNow();
    return;
  }
  loading_queue_.push_back(url_data);
}
