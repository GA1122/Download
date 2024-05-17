bool ShouldQuicRaceCertVerification(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params, "race_cert_verification"), "true");
}
