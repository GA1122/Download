bool ShouldQuicGoAwaySessionsOnIpChange(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params, "goaway_sessions_on_ip_change"),
      "true");
}
