bool ShouldMarkQuicBrokenWhenNetworkBlackholes(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params,
                        "mark_quic_broken_when_network_blackholes"),
      "true");
}
