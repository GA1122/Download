bool ShouldSupportIetfFormatQuicAltSvc(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params, "support_ietf_format_quic_altsvc"),
      "true");
}
