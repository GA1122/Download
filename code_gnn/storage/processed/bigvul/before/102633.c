int64 CloudPolicyController::GetRefreshDelay() {
  int64 deviation = (kPolicyRefreshDeviationFactorPercent *
                     policy_refresh_rate_ms_) / 100;
  deviation = std::min(deviation, kPolicyRefreshDeviationMaxInMilliseconds);
  return policy_refresh_rate_ms_ - base::RandGenerator(deviation + 1);
}
