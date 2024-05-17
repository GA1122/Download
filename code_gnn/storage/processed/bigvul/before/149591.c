LoadingStatsCollector::LoadingStatsCollector(
    ResourcePrefetchPredictor* predictor,
    const LoadingPredictorConfig& config)
    : predictor_(predictor),
      max_stats_age_(base::TimeDelta::FromSeconds(
          config.max_navigation_lifetime_seconds)) {}
