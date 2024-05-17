bool ShouldQuicRetryOnAlternateNetworkBeforeHandshake(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params,
                        "retry_on_alternate_network_before_handshake"),
      "true");
}
