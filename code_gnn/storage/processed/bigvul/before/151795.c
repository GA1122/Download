void BackgroundLoaderOffliner::ObserveResourceLoading(
    ResourceLoadingObserver::ResourceDataType type,
    bool started) {

  RequestStats& found_stats = stats_[type];
  if (started)
    ++found_stats.requested;
  else
    ++found_stats.completed;
}
