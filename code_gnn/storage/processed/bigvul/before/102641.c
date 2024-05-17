void CloudPolicyController::SetRefreshRate(int64 refresh_rate_milliseconds) {
  policy_refresh_rate_ms_ = refresh_rate_milliseconds;

  if (state_ == STATE_POLICY_VALID)
    SetState(STATE_POLICY_VALID);
}
