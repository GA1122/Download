bool ShouldQuicRaceStaleDNSOnConnection(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params, "race_stale_dns_on_connection"),
      "true");
}
