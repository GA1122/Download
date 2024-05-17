int GetRoundedBackoff(double retry_interval, double jitter) {
  const double kInitialRetryInterval = 3.0;
  const double kMinRetryInterval = 2.0;
  const double kMaxRetryInterval = 20.0;
  const double kBackoffExponent = 2.0;
  const double kMaxJitter = 0.5;

  return static_cast<int>(
      RegistrationManager::CalculateBackoff(retry_interval,
                                            kInitialRetryInterval,
                                            kMinRetryInterval,
                                            kMaxRetryInterval,
                                            kBackoffExponent,
                                            jitter,
                                            kMaxJitter));
}
