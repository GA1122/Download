bool ShouldEnableServerPushCancelation(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params, "enable_server_push_cancellation"),
      "true");
}
