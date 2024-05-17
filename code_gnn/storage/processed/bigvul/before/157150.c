void UrlIndex::RemoveLoading(UrlData* url_data) {
  auto i = loading_.find(url_data);
  if (i == loading_.end())
    return;
  loading_.erase(i);
  while (loading_.size() < GetMaxParallelPreload() && !loading_queue_.empty()) {
    auto url_data = loading_queue_.front();
    loading_queue_.pop_front();
    if (url_data->IsPreloading()) {
      WaitToLoad(url_data.get());
    } else {
      url_data->LoadNow();
    }
  }
}
