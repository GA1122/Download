void LoadingStatsCollector::RecordPageRequestSummary(
    const PageRequestSummary& summary) {
  const GURL& initial_url = summary.initial_url;

  PreconnectPrediction preconnect_prediction;
  if (predictor_->PredictPreconnectOrigins(initial_url, &preconnect_prediction))
    ReportPreconnectPredictionAccuracy(preconnect_prediction, summary);

  auto it = preconnect_stats_.find(initial_url);
  if (it != preconnect_stats_.end()) {
    ReportPreconnectAccuracy(*it->second, summary.origins);
    preconnect_stats_.erase(it);
  }
}
