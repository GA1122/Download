void LoadingPredictor::PreconnectFinished(
    std::unique_ptr<PreconnectStats> stats) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  if (shutdown_)
    return;

  DCHECK(stats);
  active_hints_.erase(stats->url);
  stats_collector_->RecordPreconnectStats(std::move(stats));
}
