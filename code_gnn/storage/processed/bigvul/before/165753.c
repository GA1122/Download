bool ShouldQuicMigrateSessionsOnNetworkChangeV2(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params,
                        "migrate_sessions_on_network_change_v2"),
      "true");
}
