bool ShouldQuicAllowServerMigration(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params, "allow_server_migration"), "true");
}
