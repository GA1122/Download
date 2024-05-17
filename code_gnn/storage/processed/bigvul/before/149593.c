void ReportPreconnectPredictionAccuracy(const PreconnectPrediction& prediction,
                                        const PageRequestSummary& summary) {
  if (prediction.requests.empty() || summary.origins.empty())
    return;

  const auto& actual_origins = summary.origins;

  size_t correctly_predicted_count = std::count_if(
      prediction.requests.begin(), prediction.requests.end(),
      [&actual_origins](const PreconnectRequest& request) {
        return actual_origins.find(request.origin) != actual_origins.end();
      });
  size_t precision_percentage =
      (100 * correctly_predicted_count) / prediction.requests.size();
  size_t recall_percentage =
      (100 * correctly_predicted_count) / actual_origins.size();

  UMA_HISTOGRAM_PERCENTAGE(
      internal::kLoadingPredictorPreconnectLearningPrecision,
      precision_percentage);
  UMA_HISTOGRAM_PERCENTAGE(internal::kLoadingPredictorPreconnectLearningRecall,
                           recall_percentage);
  UMA_HISTOGRAM_COUNTS_100(internal::kLoadingPredictorPreconnectLearningCount,
                           prediction.requests.size());

  RedirectStatus redirect_status = GetPredictionRedirectStatus(
      summary.initial_url, summary.main_frame_url, prediction.host,
      prediction.is_redirected, true  );

  UMA_HISTOGRAM_ENUMERATION(
      internal::kLoadingPredictorPreconnectLearningRedirectStatus,
      static_cast<int>(redirect_status), static_cast<int>(RedirectStatus::MAX));
}
