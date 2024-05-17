void LoadingDataCollector::RecordMainFrameLoadComplete(
    const NavigationID& navigation_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (predictor_)
    predictor_->StartInitialization();

  auto nav_it = inflight_navigations_.find(navigation_id);
  if (nav_it == inflight_navigations_.end())
    return;

  std::unique_ptr<PageRequestSummary> summary = std::move(nav_it->second);
  inflight_navigations_.erase(nav_it);

  if (stats_collector_)
    stats_collector_->RecordPageRequestSummary(*summary);

  if (predictor_)
    predictor_->RecordPageRequestSummary(std::move(summary));
}
