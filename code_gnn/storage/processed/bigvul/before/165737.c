int GetQuicMaxTimeOnNonDefaultNetworkSeconds(
    const VariationParameters& quic_trial_params) {
  int value;
  if (base::StringToInt(
          GetVariationParam(quic_trial_params,
                            "max_time_on_non_default_network_seconds"),
          &value)) {
    return value;
  }
  return 0;
}
