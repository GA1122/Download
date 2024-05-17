LoadingDataCollector::LoadingDataCollector(
    ResourcePrefetchPredictor* predictor,
    predictors::LoadingStatsCollector* stats_collector,
    const LoadingPredictorConfig& config)
    : predictor_(predictor),
      stats_collector_(stats_collector),
      config_(config) {}
