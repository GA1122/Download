  std::unique_ptr<PreconnectPrediction> GetPreconnectPrediction(
      const GURL& url) {
    auto prediction = std::make_unique<PreconnectPrediction>();
    bool has_prediction = loading_predictor_->resource_prefetch_predictor()
                              ->PredictPreconnectOrigins(url, prediction.get());
    if (!has_prediction)
      return nullptr;
    return prediction;
  }
