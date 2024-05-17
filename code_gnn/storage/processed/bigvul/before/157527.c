size_t TestDataReductionProxyConfig::GetWarmupURLFetchAttemptCounts() const {
  if (!previous_attempt_counts_)
    return DataReductionProxyConfig::GetWarmupURLFetchAttemptCounts();
  return previous_attempt_counts_.value();
}
