bool ShouldQuicGoawayOnPathDegrading(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params, "go_away_on_path_degrading"),
      "true");
}
