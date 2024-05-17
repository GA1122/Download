double RegistrationManager::CalculateBackoff(
    double retry_interval,
    double initial_retry_interval,
    double min_retry_interval,
    double max_retry_interval,
    double backoff_exponent,
    double jitter,
    double max_jitter) {
  double scaled_jitter = jitter * max_jitter;
  double new_retry_interval =
      (retry_interval == 0.0) ?
      (initial_retry_interval * (1.0 + scaled_jitter)) :
      (retry_interval * (backoff_exponent + scaled_jitter));
  return std::max(min_retry_interval,
                  std::min(max_retry_interval, new_retry_interval));
}
