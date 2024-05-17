DataReductionProxyConfigServiceClient::CalculateNextConfigRefreshTime(
    bool fetch_succeeded,
    const base::TimeDelta& config_expiration_delta,
    const base::TimeDelta& backoff_delay) {
  DCHECK(backoff_delay >= base::TimeDelta());

#if defined(OS_ANDROID)
  foreground_fetch_pending_ = false;
  if (!fetch_succeeded &&
      !base::FeatureList::IsEnabled(
          features::kDataReductionProxyAggressiveConfigFetch) &&
      IsApplicationStateBackground()) {
    foreground_fetch_pending_ = true;
    return std::max(backoff_delay, base::TimeDelta::FromSeconds(
                                       kMaxBackgroundFetchIntervalSeconds));
  }
#endif

  if (fetch_succeeded) {
    return std::max(backoff_delay, config_expiration_delta);
  }

  return backoff_delay;
}
