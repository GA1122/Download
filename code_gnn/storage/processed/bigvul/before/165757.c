bool ShouldRetryWithoutAltSvcOnQuicErrors(
    const VariationParameters& quic_trial_params) {
  return !base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params,
                        "retry_without_alt_svc_on_quic_errors"),
      "false");
}
