void AppCacheUpdateJob::CancelAllUrlFetches() {
  for (auto& pair : pending_url_fetches_)
    delete pair.second;

  url_fetches_completed_ +=
      pending_url_fetches_.size() + urls_to_fetch_.size();
  pending_url_fetches_.clear();
  urls_to_fetch_.clear();
}
