bool ShouldQuicEstimateInitialRtt(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params, "estimate_initial_rtt"), "true");
}
